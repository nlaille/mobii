#ifndef DIRECTCONNECTIONLISTENER_H_
#define DIRECTCONNECTIONLISTENER_H_

#include		<qobject.h>
#include		<qtcpserver.h>
#include		<qtcpsocket.h>

#include		"AModuleDependencyUser.h"
#include		"NetworkPool.h"

/*! \file */

/*! \class DirectConnectionListener
 *  \brief Provides client-to-mobile direct connection functionnality.
 *
 *  By default, the Mobii server is used as a gateway for all desktop clients and mobile apps of the Mobii platform; all messages are routed through this server before reaching their final destination.
 *
 *	However, when a client and a mobile try to connect to each other from the same network location (i.e. same LAN), it is more efficient to 
 *	allow them to communicate directly within their network, instead of routing messages through the server, which is a waste of time and bandwidth in this specific situation.
 *
 *	DirectConnectionListener is the module in charge of ensuring connectivity between two Mobii peers who are located on the same network location.
 */
class			DirectConnectionListener : public QObject, public AModuleDependencyUser
{
	Q_OBJECT

public:
	DirectConnectionListener(void);
	virtual ~DirectConnectionListener(void);

	virtual void OnEventBusSet(void);

	/// \brief Makes the server socket to start listening on the desired port.
	/// If the port is unavailable, the listener will try to find another port by itself.
	/// \param	port	Port on which the client should listen for direct connections.
	bool		startListeningOnPort(quint16 port);

	/// Tells the listener to stop listening on the previously specified port.
	void		stopListening(void);

	/// \brief Returns the current port on which the listener is listening.
	/// \return The port on which the client is listening for direct connections, otherwise 0 if it is not listening.
	quint16		serverPort(void);

	/// Tells if a phone has successfully established a connection to this client.
	bool		isDirectConnectionAvailable(void);

	/// \brief Writes the given datas to the directly connected phone.
	/// \return True if all given datas were sent successfully, False if the socket was not ready or if not all datas were sent.
	bool		write(const QByteArray &);

private:
	/// Direct connections listener socket.
	QTcpServer	m_listenSocket;

	/// Stores the socket on which the connected phone is bound.
	QTcpSocket *m_connectedPhoneSocket;

	/// Pool which stacks incoming stream from the phone.
	NetworkPool	m_netpool;

	/// Resets the direct connection, allowing a fresh start
	void		resetPointToPointConnection(void);

private slots:
	/// Handles events from the event bus
	void		OnEventReceived(const QString, void *, void *);

	/// Handles network-level connection failures when a phone is trying to connect directly to the client.
	void		Slot_acceptError(QAbstractSocket::SocketError);

	/// Handles new direct connections from a phone.
	void		Slot_newConnection(void);
	
	/// Triggered when the socket established between the client and the directly connected phone is ready for use.
	void		Slot_connectedPhoneReadyRead(void);

	/// Triggered when an error occurs in the direct link after the connection has been established.
	void		Slot_connectedPhoneError(QAbstractSocket::SocketError);
};


#endif // !DIRECTCONNECTIONLISTENER_H_