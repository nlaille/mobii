#include "SmartPhoneProtocol.h"

#include "session/ScopeLockSession.h"
#include "exception/IdentifierAlreadyUsed.h"

#include "PacketFactory.h"
#include "helper/Json.h"

SmartPhoneProtocol::SmartPhoneProtocol(SessionManager& sessionManager, CommunicationManager& communicationManager)
   : m_sessionManager(sessionManager), m_communicationManager(communicationManager)
{
   addFunc(Protocol::Opcode::SmartPhone::ACCEPT_COMMUNICATION, &SmartPhoneProtocol::acceptConnection);
   addFunc(Protocol::Opcode::SmartPhone::DECLINE_COMMUNICATION, &SmartPhoneProtocol::declineConnection);
}


SmartPhoneProtocol::~SmartPhoneProtocol(void)
{
}

void  SmartPhoneProtocol::acceptConnection(ISession::session& source, ISession::session& destination, Packet& packet)
{
   if (!m_communicationManager.exists(source, destination))
   {
      m_communicationManager.add(source, destination);

      Packet   openCommunicationPacket = PacketFactory::create(Protocol::Opcode::COMMUNICATION_HANDLER, Protocol::Opcode::Communication::OPEN_COMMUNICATION);

      source->sendFromTo(destination, openCommunicationPacket);
	  if (source->getConnection()->isSameNetwork(destination->getConnection()))
	  {
		  boost::property_tree::ptree ipPortJson;

	      ipPortJson.put<std::string>("ip", destination->getConnection()->getIP());
		  ipPortJson.put<unsigned int>("port", destination->getConnection()->getPort());
		  openCommunicationPacket.setMessage(Helper::Json::produce(ipPortJson));
	  }
      destination->sendFromTo(source, openCommunicationPacket);
   }
   else
	   throw std::runtime_error("[SmartPhoneProtocol] Communication already exists");
}

void  SmartPhoneProtocol::declineConnection(ISession::session& source, ISession::session& destination, Packet& packet)
{
   source->sendFromTo(destination, packet);
}

void  SmartPhoneProtocol::operator()(IConnection::connection connection, Packet& packet)
{
   std::map<char, SmartPhoneProtocolFunc>::const_iterator            it;
   ISession::session                                                 source;
   ISession::session                                                 destination;

   if (!m_sessionManager.valid(connection, packet.getSource()))
	   throw std::logic_error("[SmartPhoneProtocol] Bad ID");
   it = m_funcs.find(packet.getOpcode());
   if (it == m_funcs.end())
      throw std::logic_error("[SmartPhoneProtocol] Opcode doesn't exists");

   source = m_sessionManager.get(packet.getSource());
   destination = m_sessionManager.get(packet.getDestination());

   if (source == destination)
	   throw std::logic_error("[SmartPhoneProtocol] Same ID");
   if (!source->is(ISession::SMARTPHONE) || !destination->is(ISession::USER))
      throw std::logic_error("Bad Type : should be source[SMARTPHONE] and destination[USER]");
   ScopeLockSession  lockSource(source);
   ScopeLockSession  lockDestination(destination);

   

   (this->*((*it).second))(source, destination, packet);
}

void  SmartPhoneProtocol::addFunc(char opcode, SmartPhoneProtocolFunc func)
{
   m_funcs.insert(std::make_pair(opcode, func));
}
