#pragma once

/**
 * \file ProtocolQueue.h
 * \brief Declaration of ProtocolQueue
 * \author Nicolas Laille
 * \version 1.0
 * \date 24 Octobre 2013
 */

#include <string>

#include "network/IConnection.h"
#include "Packet.h"
#include "safe/Queue.h"

/**
 * \class ProtocolQueue
 * \brief Singleton of Queue of packet received and waiting for being handle
 */
class ProtocolQueue : public Queue<std::pair<IConnection::connection, Packet> >
{
public:
   /**
    * \fn static ProtocolQueue&  instance()
    * \brief Get instance
	* \return instance
	*/
   static ProtocolQueue&  instance()
   {
      static ProtocolQueue   instance;

      return instance;
   }
private:
   explicit ProtocolQueue() : Queue<std::pair<IConnection::connection, Packet> >() {};
};