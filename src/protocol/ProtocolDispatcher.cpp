#include "ProtocolDispatcher.h"

#include "InternalProtocol.h"
#include "RegisterProtocol.h"
#include "UserProtocol.h"
#include "SmartPhoneProtocol.h"
#include "CommunicationProtocol.h"
#include "DatabaseProtocol.h"

#include "ProtocolQueue.h"
#include "PacketFactory.h"

#include "exception/UnableToLock.h"

#include <boost/algorithm/hex.hpp>


ProtocolDispatcher::ProtocolDispatcher(SessionManager& sessionManager, CommunicationManager& communicationManager, DatabaseConnector& databaseConnector, UserManager& userManager)
   : m_isAlive(true), m_sessionManager(sessionManager), m_handlers()
{
   m_handlers.insert(std::make_pair(static_cast<char>(Protocol::Opcode::SERVER_HANDLER), new InternalProtocol(sessionManager, communicationManager, userManager)));
   m_handlers.insert(std::make_pair(static_cast<char>(Protocol::Opcode::REGISTER_HANDLER), new RegisterProtocol(sessionManager, databaseConnector)));
   m_handlers.insert(std::make_pair(static_cast<char>(Protocol::Opcode::PHONE_HANDLER), new SmartPhoneProtocol(sessionManager, communicationManager)));
   m_handlers.insert(std::make_pair(static_cast<char>(Protocol::Opcode::CLIENT_HANDLER), new UserProtocol(sessionManager, communicationManager, databaseConnector, userManager)));
   m_handlers.insert(std::make_pair(static_cast<char>(Protocol::Opcode::COMMUNICATION_HANDLER), new CommunicationProtocol(sessionManager, communicationManager)));
   m_handlers.insert(std::make_pair(static_cast<char>(Protocol::Opcode::DATABASE_HANDLER), new DatabaseProtocol(sessionManager, databaseConnector)));
}

ProtocolDispatcher::ProtocolDispatcher(const ProtocolDispatcher& protocolDispatcher)
   : m_isAlive(protocolDispatcher.m_isAlive), m_sessionManager(protocolDispatcher.m_sessionManager), 
   m_handlers(protocolDispatcher.m_handlers)
{
}

ProtocolDispatcher::~ProtocolDispatcher(void)
{
   m_isAlive = false;
   m_thread.join();
}

void  ProtocolDispatcher::start(void)
{
   this->m_thread = boost::thread(&ProtocolDispatcher::handle_, this);
}

void  ProtocolDispatcher::handle_(void)
{
   while (this->m_isAlive)
   {
      std::pair<IConnection::connection, Packet>   pair;
      std::map<char, IProtocolHandler*>::iterator  it;
      Packet                                       packet;

      pair = ProtocolQueue::instance().pop();
      packet = pair.second;
      std::cerr << packet;

      try
      {
		 it = this->m_handlers.find(packet.getDispatch());
         if (it == this->m_handlers.end())
         {
            std::cerr << "[ProtocolDispatcher] Opcode [" << packet.getDispatch() << "] doesn't exists" << std::endl;
            continue;
         }

         (*((*it).second))(pair.first, pair.second);
      }
      catch (const UnableToLock& unableToLock)
      {
         ProtocolQueue::instance().push(pair);
         std::cerr << "[ProtocolDispatcher] Error : " << unableToLock.what() << std::endl;
      }
      catch (const std::exception& exception)
      {
         std::cerr << "[ProtocolDispatcher] Error : " << exception.what() << std::endl;

		 Packet		packetException = PacketFactory::create(Protocol::Opcode::SERVER_HANDLER, Protocol::Opcode::Server::EXCEPTION);

		 (pair.first)->write(packetException.toString());
      }
   }
}