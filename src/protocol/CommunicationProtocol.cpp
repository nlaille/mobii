#include "CommunicationProtocol.h"

#include "Protocol.h"
#include "session/ScopeLockSession.h"

#include <boost/algorithm/hex.hpp>

CommunicationProtocol::CommunicationProtocol(SessionManager& sessionManager, CommunicationManager& communicationManager)
   : m_sessionManager(sessionManager), m_communicationManager(communicationManager)
{
   addFunc(Protocol::Opcode::Communication::CLOSE_COMMUNICATION, &CommunicationProtocol::closeCommunication);
   addFunc(Protocol::Opcode::Communication::REDIRECT_PACKET, &CommunicationProtocol::redirectPacket);
}


CommunicationProtocol::~CommunicationProtocol(void)
{
}

void  CommunicationProtocol::closeCommunication(ISession::session source, ISession::session destination, Packet& packet)
{
   m_communicationManager.close(source, destination);
   source->sendFromTo(destination, packet);
}

void  CommunicationProtocol::redirectPacket(ISession::session source, ISession::session destination, Packet& packet)
{
   source->sendFromTo(destination, packet);
}

void  CommunicationProtocol::operator()(IConnection::connection connection, Packet& packet)
{
   std::map<char, CommunicationProtocolFunc>::const_iterator   it;
   ISession::session                                           source;
   ISession::session                                           destination;

   if (!m_sessionManager.valid(connection, packet.getSource()))
	   throw std::logic_error("[CommunicationProtocol] Bad ID");
   it = m_funcs.find(packet.getOpcode());
   if (it == m_funcs.end())
      throw std::logic_error("[CommunicationProtocol] Opcode doesn't exists");

   source = m_sessionManager.get(packet.getSource());
   destination = m_sessionManager.get(packet.getDestination());

   if (source == destination)
	   throw std::logic_error("[CommunicationProtocol] Same ID");
   ScopeLockSession  lockSource(source);
   ScopeLockSession  lockDestination(destination);

   if (!m_communicationManager.exists(source, destination))
      throw std::logic_error("No communication open between [" + boost::algorithm::hex(packet.getSource()) + "] and [" + boost::algorithm::hex(packet.getDestination()) + "]");

   (this->*((*it).second))(source, destination, packet);
}

void  CommunicationProtocol::addFunc(char opcode, CommunicationProtocolFunc func)
{
   m_funcs.insert(std::make_pair(opcode, func));
}