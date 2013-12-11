/*
** Mobii Project -- version 0.0.1000 DEVELOPMENT VERSION
**
** Copyright(C) 2012 Mobii contributors.
**
** This library is provided `as is' and without any warranty.
** Authors decline any responsibility for any damage directly or
** indirectly caused by the use of this library.
*/

/*! \file */

#ifndef INPUTCOMMANDSPROCESSOR_H_
#define INPUTCOMMANDSPROCESSOR_H_

#include <qobject.h>

#include "commons.h"
#include "MobiiStandardPacket.h"
#include "IBaseNetworkingEmitter.h"
#include "IEventBusClient.h"
#include "ConfigurationFileIo.h"

/*! \class CommandsProcessor
 *  \brief Business logic in charge of handling input and output operations related to the Mobii protocol.
 *	The sole purpose of this class is to implement the basic behavior for each of the Mobii client-server protocol's operation codes.
 *
 *	Methods in this class shouldn't be able to perform direct I/O or business action. Their actions must be defined in events exchanged
 *	through the events bus.
 */
class CommandsProcessor : public QObject
{
	Q_OBJECT

public:
	CommandsProcessor(void);
	virtual ~CommandsProcessor(void);

	/// Sets a pointer to the global events bus.
	/// \param eventbus		Pointer to the events bus module.
	void	setEventBus(IEventBusClient *eventbus);

	//
	// Supported input commands
	//

	/// \brief Input: server has reported an error.
	/// \param packet Mobii packet which contains useful datas to process this event.
	void	do_handleServerError(const MobiiStandardPacket &);

	/// \brief Input: registration acknowledgment.
	/// \param packet Mobii packet which contains useful datas to process this event.
	void	do_handleRegistrationAck(const MobiiStandardPacket &);

	/// \brief Input: mobile authentication succeeded.
	/// \param packet Mobii packet which contains useful datas to process this event.
	void	do_processAuthSuccess(const MobiiStandardPacket &);
	
	/// \brief Input: mobile authentication failed.
	/// \param packet Mobii packet which contains useful datas to process this event.
	void	do_processAuthFailure(const MobiiStandardPacket &);
	
	/// \brief Input: user account authentication succeeded.
	/// \param packet Mobii packet which contains useful datas to process this event.
	void	do_userLoginSuccess(const MobiiStandardPacket &);

	/// \brief Input: user account authentication failed.
	/// \param packet Mobii packet which contains useful datas to process this event.
	void	do_userLoginFailure(const MobiiStandardPacket &);

	/// \brief Input: action related to user's mobiles history
	/// \param packet Mobii packet which contains useful datas to process this event.
	void	do_mobileHistoryAction(const MobiiStandardPacket &);

	/// \brief Input: does the same as do_handleJsonData(), but from a Mobii TCP packet.
	/// \param packet Mobii packet which contains useful datas to process this event.
	void	do_getMessageFromMobile(const MobiiStandardPacket &);

	/// \brief Input: the connected cellphone sent a JSON data structure (via UDP).
	/// \param packet Mobii packet which contains useful datas to process this event.
	void	do_handleJsonData(const QByteArray &);
	
	/// \brief Input: required informations to perform UDP hole punching.
	/// \param packet Mobii packet which contains useful datas to process this event.
	/// \deprecated UDP and related hole punching support has been dropped. Method left as legacy.
	void	do_performUdpHolePunching(const MobiiStandardPacket &);

	/// \brief Input: server has confirmed UDP datagram reception
	/// \param packet Mobii packet which contains useful datas to process this event.
	/// \deprecated UDP support has been dropped. Method left as legacy.
	void	do_serverHasConfirmedUdpDatagram(const MobiiStandardPacket &);

	/// \brief Input: mobile has disconnected.
	/// \param packet Mobii packet which contains useful datas to process this event.
	void	do_mobileHasDisconnected(const MobiiStandardPacket &);

	/// \brief Input: client UID registration succeeded.
	/// \param packet Mobii packet which contains useful datas to process this event.
	void	do_uidRegistrationSuccess(const MobiiStandardPacket &);

	/// \brief Input: client UID registration failed.
	/// \param packet Mobii packet which contains useful datas to process this event.
	void	do_uidRegistrationFailure(const MobiiStandardPacket &);

	/// \brief Input: acknowledge received and/or missing UDP packets during an UDP P2P session
	/// \param packet Mobii packet which contains useful datas to process this event.
	/// \deprecated UDP support has been dropped. Method left as legacy.
	void	do_handleUdpAcknowledgement(const MobiiStandardPacket &);

	//
	// Supported output commands
	//

	/// \brief Output: attempt to register a client session with server.
	/// \param authtoken	Authentication token which has been issued upon the first client connection to the server.
	/// \param directAddr	IP address of the Mobii server.
	/// \param directPort	Port on which the remote Mobii server is listening on.
	MobiiStandardPacket			do_registerClientWithServer(int authtoken, const char *directAddr, quint16 directPort);

	/// \brief Output: send login informations related to a specific account to the server.
	/// \param username		User's login name.
	/// \param password		User's associated password.
	MobiiStandardPacket			do_attemptLoginClient(const QString &username, const QString &password);

	/// \brief Output: send a mobile history-related request to server.
	/// \param data			Request parameters regarding the mobiles history request (see protocol details).
	MobiiStandardPacket			do_sendMobileHistoryRequest(const QVariant &);

	/// \brief Send a connection request to a mobile phone.
	/// The password is given as a clear string, which is then encoded using MD5 before sending to the remote phone.
	/// \param deviceId		Unique Mobii device identifier, as a 6-byte long string
	/// \param password		Clear password
	MobiiStandardPacket			do_authenticateWithMobile(const QByteArray &, const QString &);
	
	/// \brief Output: send an arbitrary message to a connected mobile phone.
	/// \param deviceId		Unique Mobii device identifier, as a 6-byte long string
	/// \param datas		Raw message to send to the phone. Mobii protocol requires `datas` to be a key-value map (e.g.: QVariantMap).
	MobiiStandardPacket			do_sendMessageToMobile(const QByteArray &deviceId, QVariant &datas);

	/// \brief Output: ask server some informations to perform hole punching (UDP).
	/// \param deviceId		Unique Mobii device identifier, as a 6-byte long string
	/// \deprecated NAT hole punching support has been dropped. Method left as a legacy.
	MobiiStandardPacket			do_requestUdpHolePunching(const QByteArray &deviceId);

	/// \brief Output: request client UID registration with server.
	MobiiStandardPacket			do_requestClientUidRegistration(void);

	/// \brief Output: acknowledge received and/or missing UDP packets during an UDP P2P session
	/// \param deviceId			Unique Mobii device identifier, as a 6-byte long string
	/// \param ackPacket		Contains a comma-separated list of UDP packets which have been acknowledged by the UDP network stack.
	/// \deprecated NAT hole punching support has been dropped. Method left as a legacy.
	MobiiStandardPacket			do_sendUdpAcknowledgement(const QByteArray &, const QString &);

private:
	/// Allows access to the Mobii's global configuration
	ConfigurationFileIo			m_cfgfile;

	/// Allows subscription to the events bus
	IEventBusClient *			eventbus_;
};

#endif // INPUTCOMMANDSPROCESSOR_H_