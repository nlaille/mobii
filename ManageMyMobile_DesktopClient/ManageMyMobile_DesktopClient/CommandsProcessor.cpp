#include <string.h>

#include <qdebug.h>
#include <qcryptographichash.h>
#include <qbytearray.h>
#include <qstringlist.h>
#include <qcryptographichash.h>

#include "PacketBuilder.h"
#include "CommandsProcessor.h"
#include "MobiiOperationCodes.h"
#include "MobiiEventBusCodes.h"
#include "InterclientProtocolHelper.h"
#include "IEventBusClient.h"
#include "InterclientProtocolHelper.h"


CommandsProcessor::CommandsProcessor(void)
	: QObject()
{
}

CommandsProcessor::~CommandsProcessor(void)
{
}

void		CommandsProcessor::setEventBus(IEventBusClient *eventbus)
{
	this->eventbus_ = eventbus;
}


/*
 * INPUT COMMANDS
 */

void		CommandsProcessor::do_handleServerError(const MobiiStandardPacket &)
{
	this->eventbus_->emitEvent(SERVER_ERROR_REPORT, this, NULL);
}

void		CommandsProcessor::do_handleRegistrationAck(const MobiiStandardPacket &)
{
	this->eventbus_->emitEvent(CLIENT_REGISTRATION_ACK, this, NULL);
}

void		CommandsProcessor::do_processAuthSuccess(const MobiiStandardPacket &packet)
{
	// Notification of binding success
	this->eventbus_->emitEvent(BLOCKING_OPERATION_END, this, NULL);
	this->eventbus_->emitEvent(BINDING_ATTEMPT_OKAY, this, NULL);

	// Request required datas to server for hole punching
	//this->eventbus_->emitEvent(NET_REQUEST_UDPHOLEPUNCHING, this, NULL);
}

void		CommandsProcessor::do_processAuthFailure(const MobiiStandardPacket &packet)
{
	this->eventbus_->emitEvent(BLOCKING_OPERATION_END, this, NULL);
	this->eventbus_->emitEvent(BINDING_ATTEMPT_FAILED, this, NULL);
}

void		CommandsProcessor::do_userLoginSuccess(const MobiiStandardPacket &)
{
	this->eventbus_->emitEvent(BLOCKING_OPERATION_END, this, NULL);
	this->eventbus_->emitEvent(USER_LOGIN_OKAY, this, NULL);
}

void		CommandsProcessor::do_userLoginFailure(const MobiiStandardPacket &)
{
	this->eventbus_->emitEvent(BLOCKING_OPERATION_END, this, NULL);
	this->eventbus_->emitEvent(DISCONNECT_FROM_SERVER, this, NULL);	
	this->eventbus_->emitEvent(USER_LOGIN_FAILED, this, NULL);
}

void		CommandsProcessor::do_mobileHistoryAction(const MobiiStandardPacket &packet)
{
	try {
		// Convert from raw JSON to Qt-compatible data structures.
		QString dataStr = QString(packet.datas);
		qDebug() << "Mobile history JSON received:" << dataStr;

		QVariantMap json = IcpHelper::jsonToData(dataStr);
		QString jsonEventCode = json["action"].toString();
		if (jsonEventCode == "list")
			this->eventbus_->emitEvent(USER_MOBILES_LIST, this, new QVariantMap(json));
		else if (jsonEventCode == "add")
			this->eventbus_->emitEvent(USER_MOBILES_ADD, this, new QVariantMap(json));
		else if (jsonEventCode == "remove")
			this->eventbus_->emitEvent(USER_MOBILES_DEL, this, new QVariantMap(json));
		else if (jsonEventCode == "rename")
			this->eventbus_->emitEvent(USER_MOBILES_RENAME, this, new QVariantMap(json));
		else
			qDebug() << "Invalid mobile history packet received, with action" << jsonEventCode;
	} catch (const std::exception &e) {
		qDebug() << "Error in" << Q_FUNC_INFO;
		qDebug() << "Error while parsing incoming message: " << e.what();
		qDebug() << "JSON size was: " << packet.datas.size();
		qDebug() << "JSON content was: " << packet.datas;
	}
}

void		CommandsProcessor::do_getMessageFromMobile(const MobiiStandardPacket &packet)
{
	this->do_handleJsonData(packet.datas);
}

void		CommandsProcessor::do_handleJsonData(const QByteArray &json)
{
	try {
		// Convert from raw JSON to Qt-compatible data structures.
		QString dataStr = QString(json);
		QVariantMap json = IcpHelper::jsonToData(dataStr);
		QString jsonEventCode = MobiiEventBusHelper::eventNameFromAction(json["action"].toString());
		this->eventbus_->emitEvent(jsonEventCode, this, new QVariantMap(json));
	} catch (const std::exception &e) {
		qDebug() << "Error while parsing incoming message: " << e.what();
		qDebug() << "JSON size was: " << json.size();
		qDebug() << "JSON content was: " << json;
	}
}

void		CommandsProcessor::do_performUdpHolePunching(const MobiiStandardPacket &packet)
{
	try {
		// Convert from raw JSON to Qt-compatible data structures.
		QString dataStr = QString(packet.datas);

		qDebug() << Q_FUNC_INFO << "Raw data:" << Commons::hexToLitteralString(packet.datas.data(), packet.datas.size()).c_str();

		QVariantMap json = IcpHelper::jsonToData(dataStr);
		this->eventbus_->emitEvent(NET_EXECUTE_UDPHOLEPUNCHING, this, new QVariantMap(json));
	} catch (const std::exception &e) {
		qDebug() << "UDP hole punching incoming connection informations:";
		qDebug() << "Error while parsing incoming message: " << e.what();
		qDebug() << "JSON size was: " << packet.dataSize;
		qDebug() << "JSON content was: " << packet.datas;
	}
}

void		CommandsProcessor::do_serverHasConfirmedUdpDatagram(const MobiiStandardPacket &)
{
	qDebug() << "Server has confirmed reception of the UDP datagram";
	this->eventbus_->emitEvent(NET_UDPHP_SERVER_RECEIVED_DGRAM, this, NULL);
}

void		CommandsProcessor::do_mobileHasDisconnected(const MobiiStandardPacket &packet)
{
	qDebug() << "Remote phone has been disconnected.";
	this->eventbus_->emitEvent(NET_MOBILE_DISCONNECTED, this, NULL);
}

void		CommandsProcessor::do_uidRegistrationSuccess(const MobiiStandardPacket &packet)
{
	qDebug() << "Client UID registration: accepted";
	try {
		// Convert from raw JSON to Qt-compatible data structures.
		QString dataStr = QString(packet.datas);
		QVariantMap json = IcpHelper::jsonToData(dataStr);
		this->eventbus_->emitEvent(UID_REGISTRATION_OKAY, this, new QVariantMap(json));
	} catch (const std::exception &e) {
		qDebug() << "Error while parsing incoming message: " << e.what();
		qDebug() << "JSON size was: " << packet.dataSize;
		qDebug() << "JSON content was: " << packet.datas;
	}
}

void		CommandsProcessor::do_uidRegistrationFailure(const MobiiStandardPacket &)
{
	qDebug() << "Client UID registration: denied";
	this->eventbus_->emitEvent(UID_REGISTRATION_FAILED, this, NULL);
}

void		CommandsProcessor::do_handleUdpAcknowledgement(const MobiiStandardPacket &packet)
{
	qDebug() << "Received UDP ACK";
	this->eventbus_->emitEvent(NET_UDPHP_RECEIVEDACK, this, (void *)(packet.datas.data()));
}

/*
 * OUTPUT COMMANDS
 */

MobiiStandardPacket			CommandsProcessor::do_registerClientWithServer(
	int authtoken, 
	const char *directAddr, 
	quint16 directPort)
{
	QMap<QString, QVariant> logininfos;
	QByteArray uid = this->m_cfgfile.clientUid();
	logininfos.insert("uid", Commons::hexToLitteralString(uid.data(), uid.size(), "").c_str());
	logininfos.insert("password", authtoken);
	logininfos.insert("local_addr", directAddr);
	logininfos.insert("port", directPort);
	QByteArray json = IcpHelper::dataToJson(logininfos);
	MobiiStandardPacket packet = PacketBuilder::prepareNewPacket(
		"SRV_REGISTER_CLIENT",
		"\x0\x0\x0\x0\x0\x0",
		"\x0\x0\x0\x0\x0\x0",
		json);
	return packet;
}

MobiiStandardPacket			CommandsProcessor::do_attemptLoginClient(const QString &username, const QString &password)
{
	QMap<QString, QVariant> logininfos;
	logininfos.insert("login_name", username);
	logininfos.insert("password", QCryptographicHash::hash(password.toLatin1(), QCryptographicHash::Md5).toHex());
	QByteArray json = IcpHelper::dataToJson(logininfos);
	MobiiStandardPacket packet = PacketBuilder::prepareNewPacket(
		"SRV_SEND_CREDENTIALS",
		QByteArray(this->m_cfgfile.clientUid(), 6),
		"\x0\x0\x0\x0\x0\x0",
		json);
	return packet;
}

MobiiStandardPacket			CommandsProcessor::do_sendMobileHistoryRequest(const QVariant &datas)
{
	MobiiStandardPacket packet = PacketBuilder::prepareNewPacket(
		"SRV_MOBILEHISTORY",
		QByteArray(this->m_cfgfile.clientUid(), 6),
		"\x00\x00\x00\x00\x00\x00",
		IcpHelper::dataToJson(datas));
	return packet;
}

MobiiStandardPacket			CommandsProcessor::do_authenticateWithMobile(const QByteArray &deviceId, const QString &clearPassword)
{
	QMap<QString, QVariant> authinfos;
	authinfos.insert("code", QCryptographicHash::hash(clearPassword.toLatin1(), QCryptographicHash::Md5).toHex());
	QByteArray json = IcpHelper::dataToJson(authinfos);
	this->eventbus_->emitEvent(BLOCKING_OPERATION_BEGIN, this, NULL);
	MobiiStandardPacket packet = PacketBuilder::prepareNewPacket(
		"MOB_AUTHENTICATE",
		QByteArray(this->m_cfgfile.clientUid(), 6),
		deviceId.data(),
		json);
	return packet;
}

MobiiStandardPacket			CommandsProcessor::do_sendMessageToMobile(const QByteArray &deviceId, QVariant &datas)
{
	MobiiStandardPacket packet = PacketBuilder::prepareNewPacket(
		"MOB_SENDMESSAGE",
		QByteArray(this->m_cfgfile.clientUid(), 6),
		deviceId.data(),
		IcpHelper::dataToJson(datas));
	return packet;
}

MobiiStandardPacket			CommandsProcessor::do_requestUdpHolePunching(const QByteArray &deviceId)
{
	// Prepare JSON datas
	QMap<QString, QVariant> logininfos;
	QByteArray uid = this->m_cfgfile.clientUid();
	logininfos.insert("uid", Commons::hexToLitteralString(uid.data(), uid.size(), "").c_str());
	logininfos.insert("password", this->m_cfgfile.authenticationToken());
	QByteArray json = IcpHelper::dataToJson(logininfos);

	// Build output packet
	MobiiStandardPacket packet = PacketBuilder::prepareNewPacket(
		"SRC_REQ_UDPHOLEPUNCHING",
		QByteArray(this->m_cfgfile.clientUid(), 6),
		deviceId.data(),
		json);
	return packet;
}

MobiiStandardPacket			CommandsProcessor::do_requestClientUidRegistration(void)
{
	// Prepare JSON datas
	QMap<QString, QVariant> logininfos;
	QByteArray uid = this->m_cfgfile.clientUid();
	logininfos.insert("uid", Commons::hexToLitteralString(uid.data(), uid.size(), "").c_str());
	QByteArray json = IcpHelper::dataToJson(logininfos);

	// Build output packet
	MobiiStandardPacket packet = PacketBuilder::prepareNewPacket(
		"SRV_UID_REGISTER",
		"\x0\x0\x0\x0\x0\x0",
		"\x0\x0\x0\x0\x0\x0",
		json);
	qDebug() << "JSON sent for authorization req.:" << QString(json);
	return packet;
}


MobiiStandardPacket			CommandsProcessor::do_sendUdpAcknowledgement(
	const QByteArray &deviceId,
	const QString &ackData)
{
	MobiiStandardPacket packet = PacketBuilder::prepareNewPacket(
		"UDP_SENDACK",
		QByteArray(this->m_cfgfile.clientUid(), 6),
		deviceId.data(),
		ackData.toLatin1());
	return packet;
}