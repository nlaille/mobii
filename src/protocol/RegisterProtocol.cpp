#include "RegisterProtocol.h"

#include "Protocol.h"
#include "PacketFactory.h"

#include "session/SmartPhoneSession.h"
#include "session/UserSession.h"
#include "session/ScopeLockSession.h"

#include "helper/Json.h"
#include "database/Model.h"

#include "exception/IdentifierAlreadyUsed.h"
#include "exception/SessionTypeException.h"

#include <boost/algorithm/hex.hpp>
#include <boost/algorithm/string.hpp> 

RegisterProtocol::RegisterProtocol(SessionManager& sessionManager, DatabaseConnector& databaseConnector)
   : m_sessionManager(sessionManager), m_databaseConnector(databaseConnector)
{
   addFunc(Protocol::Opcode::Register::SMARTPHONE, &RegisterProtocol::registerSmartPhone);
   addFunc(Protocol::Opcode::Register::USER, &RegisterProtocol::registerUser);
}

RegisterProtocol::~RegisterProtocol(void)
{
}

void  RegisterProtocol::registerSmartPhone(IConnection::connection connection, const std::string& id, Packet& packet)
{
   if (!isSmartPhone(boost::algorithm::hex(id)))
      unableToRegister(connection);
   std::cout << "Smartphone Registration UID[" << id << "]" << std::endl;
   ISession::session smartPhone = SmartPhoneSession::create(connection, id);
   ISession::session session = m_sessionManager.get(connection);

   m_sessionManager.remove(session);
   m_sessionManager.add(id, smartPhone);
   m_sessionManager.add(connection, smartPhone);

   Packet	registeredPacket = PacketFactory::create(Protocol::Opcode::REGISTER_HANDLER, Protocol::Opcode::Register::REGISTERED);
   smartPhone->sendTo(registeredPacket);
}

void  RegisterProtocol::registerUser(IConnection::connection connection, const std::string& id, Packet& packet)
{  
   if (isSmartPhone(boost::algorithm::hex(id)))
      unableToRegister(connection);
   ISession::session user = UserSession::create(connection, id);
   ISession::session session = m_sessionManager.get(connection);

   m_sessionManager.remove(session);
   m_sessionManager.add(id, user);
   m_sessionManager.add(connection, user);

   boost::property_tree::ptree	portJson = Helper::Json::parse(packet.getMessage());

   user->getConnection()->setIP(portJson.get<std::string>("local_addr"));
   user->getConnection()->setPort(portJson.get<unsigned int>("port"));
   

   Packet	registeredPacket = PacketFactory::create(Protocol::Opcode::REGISTER_HANDLER, Protocol::Opcode::Register::REGISTERED);
   user->sendTo(registeredPacket);
}
   
void  RegisterProtocol::operator()(IConnection::connection connection, Packet& packet)
{
   std::map<char, RegisterFunc>::const_iterator it;

   it = m_funcs.find(packet.getOpcode());
   if (it == m_funcs.end())
      throw std::logic_error("[RegisterProtocol] Opcode doesn't exists");

   if (!m_sessionManager.exists(connection))
      unableToRegister(connection);

   ISession::session session = m_sessionManager.get(connection);
  
   boost::property_tree::ptree json = Helper::Json::parse(packet.getMessage());
   std::string    uid(json.get<std::string>("uid"));

   if (m_sessionManager.exists(boost::algorithm::unhex(uid)))
   {
      connection->close();
      throw IdentifierAlreadyUsed("Identifier Already used");
   }
   if (uid.size() != (Protocol::Header::SOURCE_SIZE * 2))
      unableToRegister(connection);

   if (!isValidUidPassword(uid, json.get<int>("password")))
	   throw std::logic_error("[RegisterProtocol] uid and password doesn't match");
   ScopeLockSession  scopeLock(session);

   if (!session->is(ISession::UNREGISTERED))
      throw SessionTypeException("excepted[UNREGISTERED]");

   (this->*((*it).second))(connection, boost::algorithm::unhex(uid), packet);
}

void  RegisterProtocol::addFunc(char opcode, RegisterFunc function)
{
   m_funcs.insert(std::make_pair(opcode, function));
}

void  RegisterProtocol::unableToRegister(IConnection::connection connection)
{
   Packet   packet = PacketFactory::create(Protocol::Opcode::REGISTER_HANDLER, Protocol::Opcode::Register::UNABLE_TO_REGISTER);

   connection->write(packet.toString());
   throw std::logic_error("Unable to register");
}

bool  RegisterProtocol::isValidUidPassword(const std::string& uid, int password)
{
	try
	{
		Uid	uidStruct;
		uidStruct.uid = uid;
		uidStruct.password = password;
		m_databaseConnector.select<Uid, Uid>("SELECT uid, password FROM uids WHERE UPPER(uid) = UPPER(:uid) and password = :password", uidStruct);
		return true;
	}
	catch (const NoDataFound& exception)
	{
		return false;
	}
}

bool	RegisterProtocol::isSmartPhone(const std::string& uid)
{
	try
	{
		m_databaseConnector.select<std::string, std::string>("SELECT uid FROM smartphones WHERE UPPER(uid) = UPPER(:uid)", uid);
		return true;
	}
	catch (const NoDataFound& exception)
	{
		return false;
	}
}
