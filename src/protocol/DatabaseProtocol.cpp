#include "DatabaseProtocol.h"

#include "Protocol.h"
#include "PacketFactory.h"

#include "session/ScopeLockSession.h"

#include "database/Model.h"
#include "helper/Json.h"
#include "helper/Random.h"

#include <boost/algorithm/hex.hpp>
#include <sstream>

DatabaseProtocol::DatabaseProtocol(SessionManager& sessionManager, DatabaseConnector& databaseConnector)
   : m_sessionManager(sessionManager), m_databaseConnector(databaseConnector)
{
	addFunc(Protocol::Opcode::Database::UID, &DatabaseProtocol::registerID);
}


DatabaseProtocol::~DatabaseProtocol(void)
{
}

void  DatabaseProtocol::registerID(ISession::session source, Packet& packet)
{
	boost::property_tree::ptree	jsonTree(Helper::Json::parse(packet.getMessage()));
	std::string		uid = jsonTree.get<std::string>("uid");
	Packet			result;

	if (uid.size() != (2 * Protocol::Header::SOURCE_SIZE))
		throw std::logic_error("Uid should have 12 caracteres");
	try
	{
		m_databaseConnector.select<std::string, std::string>("SELECT uid FROM uids WHERE uid = :uid", uid);
		result = PacketFactory::create(Protocol::Opcode::DATABASE_HANDLER, Protocol::Opcode::Database::UID_ALREADY_USED);
	}
	catch (const NoDataFound&)
	{
		Uid		uidStruct;

		uidStruct.uid = uid;
		uidStruct.password = Helper::Random::generate();
		m_databaseConnector.insert("INSERT INTO uids(uid, password) VALUES(:uid, :password)", uidStruct);
		result = PacketFactory::create(Protocol::Opcode::DATABASE_HANDLER, Protocol::Opcode::Database::UID_CREATED);
		if (jsonTree.find("imei") != jsonTree.not_found() &&
			jsonTree.find("model") != jsonTree.not_found() &&
			jsonTree.find("manufacturer") != jsonTree.not_found())
		{
			SmartPhone	smartphone;

			smartphone.uid = uid;
			smartphone.imei = jsonTree.get<std::string>("imei");
			smartphone.model = jsonTree.get<std::string>("model");
			smartphone.manufacturer = jsonTree.get<std::string>("manufacturer");
			std::cerr << "Insert SmartPhone to base ..." << std::endl;
			m_databaseConnector.insert<SmartPhone>("INSERT INTO smartphones(uid, imei, model, manufacturer) VALUES(:uid, :imei, :model, :manufacturer)", smartphone);
			std::cerr << "Insert SmartPhone to base : Successed" << std::endl;
		}
		boost::property_tree::ptree json;

		json.put<std::string>("uid", uidStruct.uid);
		json.put<int>("password", uidStruct.password);
		result.setMessage(Helper::Json::produce(json));
	}
	source->sendTo(result);
}

void  DatabaseProtocol::operator()(IConnection::connection connection, Packet& packet)
{
   std::map<char, DatabaseProtocolFunc>::const_iterator		   it;
   ISession::session                                           source;

   it = m_funcs.find(packet.getOpcode());
   if (it == m_funcs.end())
      throw std::logic_error("[DatabaseProtocol] Opcode doesn't exists");

   source = m_sessionManager.get(connection);

   ScopeLockSession  lockSource(source);

   (this->*((*it).second))(source, packet);
}

void  DatabaseProtocol::addFunc(char opcode, DatabaseProtocolFunc func)
{
   m_funcs.insert(std::make_pair(opcode, func));
}