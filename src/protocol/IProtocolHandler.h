#pragma once

/**
 * \file IProtocolHandler.h
 * \brief Declaration of IProtocolHandler
 * \author Nicolas Laille
 * \version 1.0
 * \date 24 Octobre 2013
 */

#include "network/IConnection.h"
#include "Packet.h"

/**
 * \class IProtocolHandler
 * \brief Interface for protocole handler
 */
class IProtocolHandler
{
public:
   virtual ~IProtocolHandler() {}

   virtual void     operator()(IConnection::connection connection, Packet& packet) = 0;
};