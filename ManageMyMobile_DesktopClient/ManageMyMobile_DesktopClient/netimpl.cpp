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

#include <iostream>
#include <qsslsocket.h>
#include <qsslkey.h>
#include <qfile.h>
#include <stdexcept>
#include <qdebug.h>
#include <qthread.h>

#include "netimpl.h"
#include "commons.h"

NetImpl::NetImpl(void)
{
	// Initializes variables
	this->receiver_ = NULL;
	
	// Binds signals to appropriate slots
	QObject::connect(&(this->serverSocket_),
		SIGNAL(connected()),
		this,
		SLOT(serverHandshakeOkaySlot()));
	QObject::connect(&(this->serverSocket_),
		SIGNAL(encrypted()),
		this,
		SLOT(encryptedSlot()));
	QObject::connect(&(this->serverSocket_),
		SIGNAL(error(QAbstractSocket::SocketError)),
		this,
		SLOT(handleConnectionErrorsSlot(QAbstractSocket::SocketError)));
	QObject::connect(&(this->serverSocket_),
		SIGNAL(sslErrors(const QList<QSslError> &)),
		&(this->serverSocket_),
		SLOT(ignoreSslErrors()));
	QObject::connect(&(this->serverSocket_),
		SIGNAL(readyRead()),
		this,
		SLOT(datasReadyForRead()));
	QObject::connect(&(this->serverSocket_),
		SIGNAL(disconnected()),
		this,
		SLOT(disconnectedFromServerSlot()));
}

NetImpl::~NetImpl(void)
{
	this->receiver_ = NULL;
}

void		NetImpl::bindEventsToReceiverClass(IBaseNetworkingReceiver *receiver)
{
	this->receiver_ = receiver;
}

void		NetImpl::connectToHost(const std::string &addr, const unsigned short port, bool useSsl)
{
#ifdef CFG_NO_PEER_VERIFY
	this->serverSocket_.setPeerVerifyMode(QSslSocket::VerifyNone);
	qDebug() << "[i] SSL certificate will not be verified.";
#endif

	// Try to connect
	this->requestedSsl_ = useSsl;
	if (this->serverSocket_.state() != QAbstractSocket::UnconnectedState)
	{
		qDebug() << Q_FUNC_INFO << ":" << SOCKET_ALREADY_CONNECTED_EXCEPTION;
		throw std::runtime_error(SOCKET_ALREADY_CONNECTED_EXCEPTION);
	}
	if (useSsl == true)
		this->serverSocket_.connectToHostEncrypted(addr.c_str(), port);
	else // TCP only
		this->serverSocket_.connectToHost(addr.c_str(), port);

	// Timeout protection
#ifdef CFG_GUARD_CONNECTION_TIMEOUT
	if (this->serverSocket_.waitForConnected(5000) == false)
	{
		this->handleConnectionErrorsSlot(QAbstractSocket::SocketTimeoutError);
		this->disconnectedFromServerSlot();
	}
#endif
}

void		NetImpl::disconnectFromHost(void)
{
	if (this->serverSocket_.state() == QAbstractSocket::UnconnectedState)
	{
		qDebug() << Q_FUNC_INFO << ":" << SOCKET_ALREADY_DISCONNECTED_EXCEPTION;
		throw std::runtime_error(SOCKET_ALREADY_DISCONNECTED_EXCEPTION);
	}
	this->serverSocket_.disconnectFromHost();
}

int			NetImpl::writeToHost(const char *data, unsigned int len)
{
	if (!data || !this->isConnected())
	{
		qDebug() << "[!] Error while writing to server:";
		qDebug() << "    data = " << data;
		qDebug() << "    isEncrypted = " << this->serverSocket_.isEncrypted();
		return -1;
	}
	qDebug() << "[i] Writing " << len << " bytes";
	qDebug() << "    " << Commons::hexToLitteralString(data, len).c_str();
	return this->serverSocket_.write(data, len);
}

void		NetImpl::serverHandshakeOkaySlot(void)
{
	// Not logical, but both situations eventually lead to the same endpoint actions.
	qDebug() << "Connection first handshake okay.";
	if (this->requestedSsl_ == false)
		this->encryptedSlot();
}

void		NetImpl::encryptedSlot(void)
{
	// Call receiver's method
	if (this->receiver_)
		this->receiver_->serverHandshakeOkayReceived();
	qDebug("Connection established: [%s:%d] <========> [%s:%d]", this->serverSocket_.localAddress().toString().toStdString().c_str(), this->serverSocket_.localPort(), this->serverSocket_.peerAddress().toString().toStdString().c_str(), this->serverSocket_.peerPort());
}

void		NetImpl::handleConnectionErrorsSlot(const QList<QSslError> &sslErrors)
{
	for (QList<QSslError>::const_iterator it = sslErrors.begin(); it != sslErrors.end(); ++it)
	{
		if (*it != QSslError::NoPeerCertificate)
		{
			// Call receiver's method
			qDebug() << "SSL error:" << *it;
			Commons::S_ResponseMessage msg;
			msg.code = Commons::E_MESSAGE_CODE_SOCKET;
			msg.message = sslErrors.first().errorString().toStdString();
			msg.msgLevel = Commons::E_RESPONSE_LEVEL_CRITICAL;
			if (this->receiver_)
				this->receiver_->handleConnectionErrorsReceived(msg);
		}
		else
			qDebug() << "Notice: ignored SSL error:" << *it;
	}
}

void		NetImpl::handleConnectionErrorsSlot(QAbstractSocket::SocketError socketError)
{
	// Call receiver's method
	Commons::S_ResponseMessage msg;
	msg.code = Commons::E_MESSAGE_CODE_SOCKET;
	msg.codeBis = socketError;
	msg.message = this->serverSocket_.errorString().toStdString();
	msg.msgLevel = Commons::E_RESPONSE_LEVEL_CRITICAL;
	if (this->receiver_)
		this->receiver_->handleConnectionErrorsReceived(msg);
}

void		NetImpl::datasReadyForRead(void)
{
	QByteArray buffer;
	do
	{
		buffer = this->serverSocket_.readAll();
		if (buffer.size() != 0)
		{
			// Debug
			//qDebug() << "[i] Received " << buffer.size() << " bytes.";
			//qDebug() << "    " << Commons::hexToLitteralString(buffer, buffer.size()).c_str();
	
			// Call receiver's method
			if (this->receiver_)
				this->receiver_->incomingDataReceived(buffer.constData(), buffer.length());
		}
	}
	while (buffer.size() > 0);
}

void		NetImpl::disconnectedFromServerSlot(void)
{
	// Call receiver's method
	if (this->receiver_)
		this->receiver_->disconnectedFromServerReceived();
}

bool		NetImpl::isConnected(void)
{
	return (this->serverSocket_.state() == QAbstractSocket::ConnectedState);
}

const std::string NetImpl::localAddress(void)
{
	return this->serverSocket_.localAddress().toString().toStdString();
}

unsigned int NetImpl::localPort(void)
{
	return this->serverSocket_.localPort();
}
