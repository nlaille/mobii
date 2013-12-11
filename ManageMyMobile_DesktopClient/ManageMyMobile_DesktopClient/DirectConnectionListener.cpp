#include		<qdebug.h>
#include		<qdatetime.h>
#include		<qtcpsocket.h>

#include		"MobiiEventBusCodes.h"
#include		"DirectConnectionListener.h"


DirectConnectionListener::DirectConnectionListener(void)
	: m_connectedPhoneSocket(NULL)
{
	connect(&(this->m_listenSocket), SIGNAL(acceptError(QAbstractSocket::SocketError)), this, SLOT(Slot_acceptError(QAbstractSocket::SocketError)));
	connect(&(this->m_listenSocket), SIGNAL(newConnection()), this, SLOT(Slot_newConnection()));
}


DirectConnectionListener::~DirectConnectionListener(void)
{
}


void		DirectConnectionListener::OnEventBusSet(void)
{
	this->eventbus_->registerAsConsumer(NET_SERVER_DISCONNECTED, this, SLOT(OnEventReceived(const QString, void *, void *)));
	this->eventbus_->registerAsConsumer(NET_MOBILE_DISCONNECTED, this, SLOT(OnEventReceived(const QString, void *, void *)));
}


void		DirectConnectionListener::OnEventReceived(const QString eventName, void *, void *data)
{
	if (eventName == NET_SERVER_DISCONNECTED)
		this->stopListening();
	else if (eventName == NET_MOBILE_DISCONNECTED)
		this->resetPointToPointConnection();
}


bool		DirectConnectionListener::startListeningOnPort(quint16 port)
{
	if (this->m_listenSocket.serverPort() != 0)
	{
		qDebug() << "[DIRECT_CON] Already listening on port" << this->m_listenSocket.serverPort();
		return false;
	}

	bool listenSucceeded = this->m_listenSocket.listen(QHostAddress::Any, port);
	
	// Fallback if user-defined port is not available
	for (int i = 0; i < 5 && listenSucceeded == false; ++i)
		listenSucceeded = this->m_listenSocket.listen();
	
	qDebug() << "[DIRECT_CON] Listening on port" << this->m_listenSocket.serverPort() << (this->m_listenSocket.serverPort() == 0 ? "<FAILED>" : "");
	return listenSucceeded;
}


void		DirectConnectionListener::stopListening(void)
{
	this->m_listenSocket.close();
}


quint16		DirectConnectionListener::serverPort(void)
{
	return this->m_listenSocket.serverPort();
}


bool		DirectConnectionListener::isDirectConnectionAvailable(void)
{
	return this->m_connectedPhoneSocket != NULL && this->m_connectedPhoneSocket->isValid();
}


void		DirectConnectionListener::resetPointToPointConnection(void)
{
	if (this->m_connectedPhoneSocket != NULL)
	{
		this->m_connectedPhoneSocket->disconnectFromHost();
		disconnect(this, SLOT(Slot_connectedPhoneReadyRead()));
		disconnect(this, SLOT(Slot_connectedPhoneError(QAbstractSocket::SocketError)));
		delete this->m_connectedPhoneSocket;
		this->m_connectedPhoneSocket = NULL;
	}
	this->m_netpool.flushAll();
}


bool		DirectConnectionListener::write(const QByteArray &datas)
{
	if (!this->isDirectConnectionAvailable())
		return false;
	return (this->m_connectedPhoneSocket->write(datas) == datas.size());
}


void		DirectConnectionListener::Slot_acceptError(QAbstractSocket::SocketError socketError)
{
	qDebug() << "[DIRECT_CON] Error code" << socketError << "-" << this->m_listenSocket.errorString();
}


void		DirectConnectionListener::Slot_newConnection(void)
{
	qDebug() << "[DIRECT_CON] New pending connection, verifying...";

	// There is no way to determine which socket is allowed.
	// For now, first in, first served.
	//while (this->m_listenSocket.hasPendingConnections())
	if (this->m_connectedPhoneSocket != NULL)
		return;
	this->m_connectedPhoneSocket = this->m_listenSocket.nextPendingConnection();

	// Connect appropriate signals on this socket
	connect(this->m_connectedPhoneSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(Slot_connectedPhoneError(QAbstractSocket::SocketError)));
	connect(this->m_connectedPhoneSocket, SIGNAL(readyRead()), this, SLOT(Slot_connectedPhoneReadyRead()));
}


void		DirectConnectionListener::Slot_connectedPhoneReadyRead(void)
{
	QByteArray buffer;
	do
	{
		buffer = this->m_connectedPhoneSocket->readAll();
		this->m_netpool.feedWithRawDatas(buffer);
	}
	while (!buffer.isEmpty());
	while (this->m_netpool.isNextPacketReady())
	{
		MobiiStandardPacket *packet = new MobiiStandardPacket(this->m_netpool.nextPacket());
		this->eventbus_->emitEvent(COMMSRV_PROCESS_PACKET, this, packet);
	}
}


void		DirectConnectionListener::Slot_connectedPhoneError(QAbstractSocket::SocketError error)
{
	qDebug() << "[DIRECT_CON] Connected phone socket error - Code" << error << ":" << this->m_connectedPhoneSocket->errorString();

	// Abort connection
	this->resetPointToPointConnection();
}