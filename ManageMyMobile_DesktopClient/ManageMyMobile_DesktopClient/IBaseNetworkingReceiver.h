/*
** NetImpl Library -- version 0.0.1000 DEVELOPMENT VERSION
** QT-based network implementation for the Mobii project protocol.
**
** Copyright(C) 2012 Mobii contributors.
**
** This library is provided `as is' and without any warranty.
** Authors decline any responsibility for any damage directly or
** indirectly caused by the use of this library.
*/

#ifndef IBASENETWORKINGRECEIVER_H
#define IBASENETWORKINGRECEIVER_H

#include "commons.h"

/*! \file */

/*! \class IBaseNetworkingReceiver
 *  \brief Interface for network events recipient classes
 *
 *  A class which inherits this interface can process server related events.
 *  Such events notably include server connection and disconnection notifications,
 *  socket errors management and datas reception notification.
 */
class IBaseNetworkingReceiver
{
public:
	virtual ~IBaseNetworkingReceiver(void) {}

	/// The server agreed to open a socket with the requester client.
	/// At this step, the client is only connected to the server, but it is not registered as a Mobii client yet.
	virtual void serverHandshakeOkayReceived(void) = 0;

	/// The server has terminated the connection.
	virtual void disconnectedFromServerReceived(void) = 0;

	/// A network-related error has occured.
	virtual void handleConnectionErrorsReceived(Commons::S_ResponseMessage msg) = 0;

	/// Datas have been received from the server.
	virtual void incomingDataReceived(const char *data, unsigned int len) = 0;
};

#endif // IBASENETWORKINGRECEIVER_H