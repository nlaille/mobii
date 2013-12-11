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

/*! \file */

#ifndef NETIMPL_H
#define NETIMPL_H

// QT Includes
#include <qsslsocket.h>
#include <qudpsocket.h>
#include <qglobal.h>
#include <qlist.h>

// Internal project includes
#include "IBaseNetworkingReceiver.h"
#include "IBaseNetworkingEmitter.h"

/*
** CONFIGURATION DEFINES
*/

/*! \def CFG_NO_PEER_VERIFY
 *  \brief Socket will not attempt to check the validity of the SSL certificate.
 *
 *   By default, the SSL socket verifies the SSL certificate provided by the server.
 *   If the certificate is self-signed, has not been issued an certified authority,
 *   or presents a domain-name mismatch, the socket drops the connection and issues
 *   a security error.
 *   Defining this macro will prevent the socket to do so.
 */
#define CFG_NO_PEER_VERIFY

/*! \class NetImpl
 *  \brief QT-based low-level implementation of network socket
 *
 *  NetImpl is a low-level implementation of a network socket. It is based on QT's
 *  QSslSocket, which is QT's implementation of the TCP and SSL protocols.
 *  It only provides basic network operations, and sends signals the receiver class.
 */
class			NetImpl: QObject, public IBaseNetworkingEmitter
{
	Q_OBJECT

public:
	/*!
     *  \brief Constructor
     *
     *  Constructor for NetImpl class
     */
	NetImpl(void);

	/*!
     *  \brief Destructor
     *
     *  Destructor for NetImpl class
     */
	~NetImpl(void);

	/*!
     *  \brief Establishes a secured TCP connection to the server
     *
     *  Connects to the specified address using the specified port, using the TCP protocol.
	 *  The connection is secured using the SSL protocol.
	 *
	 *  \param addr   : address of the external server.
	 *  \param port   : connection port to use.
	 *  \param useSsl : specifies if the socket should try to connect using SSL.
     */
	virtual void	connectToHost(
						const std::string &addr,
						const unsigned short port,
						bool useSsl);

	/*!
     *  \brief Drops an existing connection to the server
     *
     *  If the socket is currently connected to the server, the connection is dropped 
	 *  and the socket becomes available for a new connection.
     */
	virtual void	disconnectFromHost(void);

	/*!
     *  \brief Writes datas to a connected peer
     *
     *  Sends the specified datas of specified length to the connected server.
	 *  If the socket is currently disconnected, this call fails and returns -1.
	 *
	 *  \param data : datas to send to the peer.
	 *  \param len : length of datas to send.
	 *  \return number of bytes written, or -1 upon failure.
     */
	virtual int		writeToHost(const char *data, unsigned int len);

	/*!
     *  \brief Tells whether the server is connected or not.
	 *
	 *  \return true if server is connected, false otherwise.
     */
	virtual bool	isConnected(void);

	virtual const std::string localAddress(void);

	virtual unsigned int localPort(void);

	/*!
     *  \brief Binds events to the appropriate receiver class
     *
     *  Specifies the class which handles socket events.
	 *  Most of the time, the appropriate class is the one which holds communication services.
	 *
	 *  \param receiver Class which handles events sent by NetImpl.
     */
	void			bindEventsToReceiverClass(IBaseNetworkingReceiver *receiver);

private:
	/// Internal TCP socket for server connectivity
	QSslSocket		serverSocket_;

	/// Pointer to the module which will handle packets from the network.
	IBaseNetworkingReceiver *receiver_;

	/// List of ignored SSL errors, typically errors which are non critical
	QList<QSslError>		ignoredSslErrors_;

	/// Specifies if user has requested SSL encryption
	bool			requestedSsl_;

private slots:
    /// Triggered when SSL connection is successful
	void			serverHandshakeOkaySlot(void);

	/// Triggered when encryption through SSL is confirmed
	void			encryptedSlot(void);

    /// Triggered when connection is dropped
	void			disconnectedFromServerSlot(void);

	/// Triggered when datas are ready to be read
	void			datasReadyForRead(void);

	/// Handles TCP-related errors
	void			handleConnectionErrorsSlot(QAbstractSocket::SocketError socketError);

	/// Handles SSL-related errors
	void			handleConnectionErrorsSlot(const QList<QSslError> &);
};

#endif // NETIMPL_H
