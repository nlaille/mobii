/*
** Mobii Project -- version 0.0.1000 DEVELOPMENT VERSION
**
** Copyright(C) 2012 Mobii contributors.
**
** This library is provided `as is' and without any warranty.
** Authors decline any responsibility for any damage directly or
** indirectly caused by the use of this library.
*/

#include <qbytearray.h>
#include <qdebug.h>
#include <qstringlist.h>
#include <qthread.h>

#include <stdexcept>
#include <sstream>

#include "commons.h"
#include "IBaseNetworkingEmitter.h"
#include "CommunicationServices.h"
#include "PacketBuilder.h"
#include "MobiiOperationCodes.h"
#include "MobiiEventBusCodes.h"
#include "IEventBusClient.h"
#include "ConfigurationFileIo.h"


CommunicationServices::CommunicationServices(
	IBaseNetworkingEmitter *emitter)
	: QObject(), readyForOperations_(false)
{
	this->setEmitterInterface(emitter);
}


CommunicationServices::~CommunicationServices(void)
{
}


void		CommunicationServices::OnEventBusSet(void)
{
	this->processor_.setEventBus(this->eventbus_);
	this->connectionListener_.setEventBus(this->eventbus_);
	
	this->eventbus_->registerAsConsumer(ATTEMPT_MOBILE_BINDING, this, SLOT(OnEventReceived(const QString, void *, void *)));
	this->eventbus_->registerAsConsumer(BINDING_ATTEMPT_OKAY, this, SLOT(OnEventReceived(const QString, void *, void *)));
	this->eventbus_->registerAsConsumer(CONNECT_TO_SERVER, this, SLOT(OnEventReceived(const QString, void *, void *)));
	this->eventbus_->registerAsConsumer(DISCONNECT_FROM_SERVER, this, SLOT(OnEventReceived(const QString, void *, void *)));
	this->eventbus_->registerAsConsumer(JSON_SEND_REQUEST, this, SLOT(OnEventReceived(const QString, void *, void *)));
	this->eventbus_->registerAsConsumer(USER_LOGIN_OKAY, this, SLOT(OnEventReceived(const QString, void *, void *)));
	this->eventbus_->registerAsConsumer(CLIENT_REGISTRATION_ACK, this, SLOT(OnEventReceived(const QString, void *, void *)));
	this->eventbus_->registerAsConsumer(NET_REQUEST_UDPHOLEPUNCHING, this, SLOT(OnEventReceived(const QString, void *, void *)));
	this->eventbus_->registerAsConsumer(NET_EXECUTE_UDPHOLEPUNCHING, this, SLOT(OnEventReceived(const QString, void *, void *)));
	this->eventbus_->registerAsConsumer(NET_SERVER_DISCONNECTED, this, SLOT(OnEventReceived(const QString, void *, void *)));
	this->eventbus_->registerAsConsumer(NET_UDPHP_SENDACK, this, SLOT(OnEventReceived(const QString, void *, void *)));
	this->eventbus_->registerAsConsumer(UID_REGISTRATION_OKAY, this, SLOT(OnEventReceived(const QString, void *, void *)));
	this->eventbus_->registerAsConsumer(UID_REGISTRATION_FAILED, this, SLOT(OnEventReceived(const QString, void *, void *)));
	this->eventbus_->registerAsConsumer(USER_MOBILES_SENDREQ, this, SLOT(OnEventReceived(const QString, void *, void *)));
	this->eventbus_->registerAsConsumer(COMMSRV_PROCESS_PACKET, this, SLOT(OnEventReceived(const QString, void *, void *)));
}


void		CommunicationServices::OnSessionControllerSet(void)
{
}


bool		CommunicationServices::connectToServer(const char *address, unsigned short port, bool useSsl)
{
	try
	{
		this->emitter_->connectToHost(address, port, useSsl);
	}
	catch (const std::runtime_error &e)
	{
		qDebug() << Q_FUNC_INFO << ":" << e.what();
		if (e.what() == SOCKET_ALREADY_CONNECTED_EXCEPTION)
			return false;
	}
	return true;
}


bool		CommunicationServices::disconnectFromServer(void)
{
	try
	{
		this->emitter_->disconnectFromHost();
	}
	catch (const std::runtime_error &e)
	{
		qDebug() << Q_FUNC_INFO << ":" << e.what();
		if (e.what() == SOCKET_ALREADY_DISCONNECTED_EXCEPTION)
			return false;
	}
	return true;
}


bool		CommunicationServices::sendMobiiPacket(const MobiiStandardPacket &packet)
{
	// Prepare raw bytes to send
	QByteArray tosend = PacketBuilder::getRawDatasFromPacket(packet);
	
	if (this->connectionListener_.isDirectConnectionAvailable())
		return this->connectionListener_.write(tosend);

	// Use the server if direct connection not available
	if (this->emitter_->writeToHost(tosend.data(), tosend.size()) == -1)
	{
		this->eventbus_->emitEvent(NET_SERVER_ERROR, this, NULL);
		return false;
	}
	return true;
}


bool		CommunicationServices::isServerConnected(void)
{
	return (this->emitter_->isConnected());
}


void		CommunicationServices::setEmitterInterface(IBaseNetworkingEmitter *emitter)
{
	if (!emitter)
	{
		qDebug() << Q_FUNC_INFO << "[!] Provided NULL emitter to Communication Services.";
		throw std::invalid_argument("[!] Provided NULL emitter to Communication Services.");
	}
	this->emitter_ = emitter;
}


void		CommunicationServices::serverHandshakeOkayReceived(void)
{
	if (this->appconfig_.serverHasValidatedUid() == false
		&& this->appconfig_.readValue("global/novalidate").toBool() != true)
	{
		// Request confirmation that UID is available for use by this client
		qDebug() << "UID not validated yet, sending confirmation request to server.";
		MobiiStandardPacket packet = this->processor_.do_requestClientUidRegistration();
		QByteArray tosend = PacketBuilder::getRawDatasFromPacket(packet);
		this->emitter_->writeToHost(tosend.data(), tosend.size());
	}
	else // Can proceed to regular registration as usual
		this->registerConfirmedClientWithServer();
}


void		CommunicationServices::registerConfirmedClientWithServer(void)
{
	// Prepare direct connection listener
	this->connectionListener_.startListeningOnPort(this->appconfig_.directConnectionPort());
	quint16 listeningPort = this->connectionListener_.serverPort();

	// Send client identity to server
	MobiiStandardPacket packet = this->processor_.do_registerClientWithServer(this->appconfig_.authenticationToken(), this->emitter_->localAddress().c_str(), listeningPort);
	QByteArray tosend = PacketBuilder::getRawDatasFromPacket(packet);
	this->emitter_->writeToHost(tosend.data(), tosend.size());
}


void		CommunicationServices::clientSessionOpenWithServer(void)
{
	qDebug("[i] Connection established to server");

	// Set internal state
	this->readyForOperations_ = true;

	// Send signal to final receiver
	this->eventbus_->emitEvent(BLOCKING_OPERATION_END, this, NULL);
	this->eventbus_->emitEvent(NET_SERVER_CONNECTED, this, NULL);
}


void		CommunicationServices::disconnectedFromServerReceived(void)
{
	this->readyForOperations_ = false;
	qDebug("[i] Disconnected from server");

	// Flush the network data pool
	this->netPool_.flushAll();

	// Send signal to final receiver
	this->eventbus_->emitEvent(NET_SERVER_DISCONNECTED, this, NULL);
}


void		CommunicationServices::handleConnectionErrorsReceived(Commons::S_ResponseMessage msg)
{
	qDebug("[!] Socket error received");
	qDebug() << "    Reason: " << msg.message.c_str();

	// Send signal to final receiver
	this->eventbus_->emitEvent(NET_SERVER_ERROR, this, new Commons::S_ResponseMessage(msg));
}


void		CommunicationServices::incomingDataReceived(const char *data,
														unsigned int len)
{
	QByteArray freshdata(data, len);
	this->netPool_.feedWithRawDatas(freshdata);
	while (this->netPool_.isNextPacketReady())
	{
		MobiiStandardPacket packet = this->netPool_.nextPacket();
		this->processMobiiPacket(packet);
	}
}


void		CommunicationServices::processMobiiPacket(const MobiiStandardPacket &packet)
{
	fCommand cmd = MobiiOpcodes::getCommandForOpcode(packet.dispatch, packet.opcode);
	if (cmd == NULL)
	{
		qDebug("Operation code not recognized: \\x%X\\x%X", packet.dispatch, packet.opcode);
		return;
	}
	(this->processor_.*cmd)(packet);
}


bool		CommunicationServices::isReadyForOperations(void)
{
	return this->readyForOperations_;
}


void		CommunicationServices::OnEventReceived(
	const QString eventName, 
	void *sender, 
	void *data)
{
	qDebug() << "Communication Services: received event `" << eventName << "'";
	if (eventName == ATTEMPT_MOBILE_BINDING)
	{
		this->eventbus_->emitEvent(BLOCKING_OPERATION_BEGIN, this, NULL);
		QPair<QByteArray, QString> *bindingDatas = (QPair<QByteArray, QString> *)(data);
		this->registry_["device_to_bind_uid"] = bindingDatas->first;
		this->registry_["device_to_bind_password"] = bindingDatas->second;

		// Send packet
		if (this->isReadyForOperations())
			this->sendMobiiPacket(this->processor_.do_authenticateWithMobile(bindingDatas->first, bindingDatas->second));
		else
		{
			QVector<QString> *args = new QVector<QString>();
			args->push_back("gateway_only");
			this->eventbus_->emitEvent(CONNECT_TO_SERVER, this, args);
		}
	}
	else if (eventName == JSON_SEND_REQUEST)
	{
		QVariantMap *jsondata = (QVariantMap *)data;
		QVariant genericData(*jsondata);
		this->sendMobiiPacket(this->processor_.do_sendMessageToMobile(this->sessioncon_->currentDevice()->id(), genericData));
	}
	else if (eventName == CONNECT_TO_SERVER)
	{
		// Read settings from configuration file
		QString servAddr = this->appconfig_.serverAddress();
		quint16 servPort = this->appconfig_.serverTcpPort();

		// Abort operation if server settings are missing
		if (servAddr.isEmpty() || servPort == 0)
			this->eventbus_->emitEvent(SERVER_SETTINGS_NOT_SET, this, NULL);
		else // Settings are okay
		{
			this->eventbus_->emitEvent(BLOCKING_OPERATION_BEGIN, this, NULL);
			QVector<QString> *args = (QVector<QString> *)(data);
			if (args->contains("gateway_only"))
			{
				// Only connect to server
				this->registry_["connection_mode"] = "gateway_only";
			}
			else
			{
				// Connect to server then attempt login
				this->registry_["connection_mode"] = "login";
				this->registry_["username"] = args->at(0);
				this->registry_["password"] = args->at(1);
			}

			// Attempts connection to server
			this->connectToServer(servAddr.toStdString().c_str(), servPort, this->appconfig_.useSsl());
		}
	}
	else if (eventName == UID_REGISTRATION_OKAY)
	{
		qDebug() << "Server has accepted usage authorization for this client UID.";
		QVariantMap *args = (QVariantMap *)(data);
		this->appconfig_.setAuthenticationToken((*args)["password"].toInt());
		this->appconfig_.setServerHasValidatedUid(true);
		this->registerConfirmedClientWithServer();
	}
	else if (eventName == UID_REGISTRATION_FAILED)
	{
		qDebug() << "Server has refused usage authorization for this client UID.";
		this->appconfig_.setServerHasValidatedUid(false);
		this->eventbus_->emitEvent(CORE_REQ_RESETUID, this, NULL);
		this->disconnectFromServer();
	}
	else if (eventName == CLIENT_REGISTRATION_ACK)
	{
		if (this->registry_["connection_mode"].toString() == "login")
			this->sendMobiiPacket(this->processor_.do_attemptLoginClient(this->registry_["username"].toString(), this->registry_["password"].toString()));
		else // gateway only
		{
			this->clientSessionOpenWithServer();
			this->sendMobiiPacket(this->processor_.do_authenticateWithMobile(
				this->registry_["device_to_bind_uid"].toByteArray(),
				this->registry_["device_to_bind_password"].toString()));
		}
	}
	else if (eventName == NET_UDPHP_SENDACK)
	{
		QString *ackData = (QString *)data;
		qDebug() << "Sending ACK packet via TCP:" << *ackData;
		this->sendMobiiPacket(this->processor_.do_sendUdpAcknowledgement(this->sessioncon_->currentDevice()->id(), *ackData));
	}
	else if (eventName == BINDING_ATTEMPT_OKAY)
	{
		this->registry_["wasPreviouslyConnected"] = true;
		this->sessioncon_->loadDevice(this->registry_["device_to_bind_uid"].toByteArray());
		this->eventbus_->emitEvent(NET_MOBILE_CONNECTED, this, NULL);
	}
	else if (eventName == USER_LOGIN_OKAY)
	{
		this->sessioncon_->loadUser(this->registry_["username"].toString());
		this->clientSessionOpenWithServer();
	}
	else if (eventName == USER_MOBILES_SENDREQ)
	{
		QVariantMap *json = (QVariantMap *)data;
		this->sendMobiiPacket(this->processor_.do_sendMobileHistoryRequest(*json));
	}
	else if (eventName == DISCONNECT_FROM_SERVER)
	{
		this->disconnectFromServer();
	}
	else if (eventName == NET_SERVER_DISCONNECTED)
	{
		// Automatic as mobile connectivity currently depends on server connection, if previous state was "connected to a mobile"
		if (this->registry_["wasPreviouslyConnected"].toBool() == true)
			this->eventbus_->emitEvent(NET_MOBILE_DISCONNECTED, this, NULL);	
		this->registry_["wasPreviouslyConnected"] = false;

		// Set user to default
		this->sessioncon_->loadUser();
	}
	else if (eventName == COMMSRV_PROCESS_PACKET)
	{
		MobiiStandardPacket *packet = (MobiiStandardPacket *)data;
		this->processMobiiPacket(*packet);
	}
	if (data != NULL)
		delete data;
}
