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

#ifndef IBASENETWORKINGEMITTER_H
#define IBASENETWORKINGEMITTER_H

/*! \file */

/*! \class IBaseNetworkingEmitter
 *  \brief Interface for network sender class
 *
 *  A class which inherits this interface can notably operate on a 
 *  socket connection and send datas to a remote peer.
 */
class IBaseNetworkingEmitter
{
public:
	virtual ~IBaseNetworkingEmitter(void) {}

	/// Establish a TCP connection to the server.
	/// Allows establishment of a SSL secured connection.
	virtual void		connectToHost(const std::string &addr, const unsigned short port, bool useSsl = false) = 0;

	/// Shuts down an existing connection.
	virtual void		disconnectFromHost(void) = 0;

	/// Writes the given datas to the server.
	virtual int			writeToHost(const char *data, unsigned int len) = 0;

	/// Tells if a connection is currently active with the server.
	virtual bool		isConnected(void) = 0;

	/// Returns the local address currently used for connection.
	virtual const std::string localAddress(void) = 0;

	/// Returns the local port currently used for connection.
	virtual unsigned int localPort(void) = 0;
};

#endif // IBASENETWORKINGEMITTER_H