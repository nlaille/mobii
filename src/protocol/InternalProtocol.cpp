#include "InternalProtocol.h"

#include "Protocol.h"

#include "session/UnregisteredSession.h"
#include "PacketFactory.h"

InternalProtocol::InternalProtocol(SessionManager& sessionManager, CommunicationManager& communicationManager, UserManager& userManager)
   : m_sessionManager(sessionManager), m_communicationManager(communicationManager), m_userManager(userManager)
{
   addFunc(Protocol::Opcode::Server::CONNECT, &InternalProtocol::connect);
   addFunc(Protocol::Opcode::Server::DISCONNECT, &InternalProtocol::disconnected);
}


InternalProtocol::~InternalProtocol(void)
{
}

void     InternalProtocol::connect(IConnection::connection connection)
{
   if (m_sessionManager.exists(connection))
      throw std::logic_error("Connection already exists");
   m_sessionManager.add(connection, UnregisteredSession::create(connection));
}

void     InternalProtocol::disconnected(IConnection::connection connection)
{
   ISession::session session = m_sessionManager.get(connection);

   Packet   closeCommunicationPacket = PacketFactory::create(Protocol::Opcode::COMMUNICATION_HANDLER, Protocol::Opcode::Communication::CLOSE_COMMUNICATION);

   m_communicationManager.sendAll(session, closeCommunicationPacket);
   m_communicationManager.closeAll(session);
   m_userManager.logout(session);
   m_sessionManager.remove(session);
   session->close();
   std::cout << "Disconnected" << std::endl;
} 

void  InternalProtocol::operator()(IConnection::connection connection, Packet& packet)
{
   std::map<char, ServerProtocolFunc>::const_iterator            it;

   it = m_funcs.find(packet.getOpcode());
   if (it == m_funcs.end())
      throw std::logic_error("[InternalProtocol] Opcode doesn't exists");
   (this->*((*it).second))(connection);
}

void  InternalProtocol::addFunc(char opcode, ServerProtocolFunc func)
{
   m_funcs.insert(std::make_pair(opcode, func));
}