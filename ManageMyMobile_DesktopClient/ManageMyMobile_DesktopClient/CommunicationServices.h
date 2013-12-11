#ifndef COMMUNICATIONSERVICES_H_
#define COMMUNICATIONSERVICES_H_

#include <qobject.h>
#include <qmap.h>

#include "commons.h"
#include "IBaseNetworkingReceiver.h"
#include "IBaseNetworkingEmitter.h"
#include "NetworkPool.h"
#include "CommandsProcessor.h"
#include "AModuleDependencyUser.h"
#include "ConfigurationFileIo.h"
#include "DirectConnectionListener.h"

/*! \file */

/*! \class CommunicationServices
 *  \brief Handles the implementation of the Mobii protocol.
 *
 *  The Communication Services class is in charge of ensuring communications with the
 *  low-level network module (aka. NetImpl), and also handles input and output
 *  commands built following the Mobii protocol.
 *
 *	This class relies on the CommandsProcessor module for the implementation of the operations codes as specified in the
 *	client-server protocol. At no moment should this class directly send raw operation codes to the server or the connected phone.
 */
class CommunicationServices : public QObject, public AModuleDependencyUser, public IBaseNetworkingReceiver
{
	Q_OBJECT

public:
	/// Requires a pointer reference to the network module in charge of sending raw datas to the network.
	explicit CommunicationServices(IBaseNetworkingEmitter *emitter);

	virtual ~CommunicationServices(void);

    /// \brief Resets the network emitter class
    /// Allows to change the emitter class after constructor call.
	/// The emitter is the network low-level class in charge of sending output commands.
	/// \param	emitter		Sets a pointer to the module which is in charge of sending the raw Mobii packets to the remote peers.
	void		setEmitterInterface(IBaseNetworkingEmitter *emitter);

	/// \brief Launches connection to the server
    /// Initiates a connection to the Mobii server.
	/// \param	address		IP address of the Mobii server.
	/// \param	port		Port on which the Mobii server is listening on.
	/// \param	useSsl		Specifies if connections should be attempted using SSL.
	/// \return	True upon success, False if the server could not be reached, connection has been denied, or a connection is already on.
	bool		connectToServer(const char *address, unsigned short port, bool useSsl);

    /// \brief Aborts an existing connection to the server.
	/// \return True upon success, False if the client is not connected to any peer.
	bool		disconnectFromServer(void);

	/// \brief Sends Mobii packets through network
    /// Allows upper layers to send packets conforming the Mobii protocol.
	/// \param	packet	Mobii packet according to the client-server protocol.
	/// \return	True if the packet was sent correctly, False otherwise.
	bool		sendMobiiPacket(const MobiiStandardPacket &);

	/// \brief Tells if a connection is currently active.
	/// \return	True is the client is already connected to the server, False otherwise.
	bool		isServerConnected(void);

private slots:
	/// \brief	Determines an action to do when a message from the global events bus is received.
	/// \param	eventName	Name of the events bus message.
	/// \param	sender		Pointer to the object which sent the message.
	/// \param	data		Raw datas associated to the sent event.
	void			OnEventReceived(const QString eventName, void *sender, void *data);

protected:
	/// Specifies actions to perform when the event bus is set.
	virtual void OnEventBusSet(void);

	/// Specifies actions to perform when the session controller is set.
	virtual void OnSessionControllerSet(void);

private:
	/// Tells if the network socket is ready for read or write operations.
	bool			readyForOperations_;

	/// Network pool which buffers raw incoming datas for unpacking.
	NetworkPool		netPool_;

	/// Processes input commands received and unpacked from the server or phone.
	CommandsProcessor processor_;

	/// Provides methods for emitting datas and packets to the server.
	IBaseNetworkingEmitter *emitter_;

	/// General configuration I/O
	ConfigurationFileIo appconfig_;

	/// Stores internal configuration and status. Kind of an internal Windows Registry.
	QMap<QString, QVariant>	registry_;

	/// Modules which provides direct phone connection capability.
	DirectConnectionListener connectionListener_;

	/// \brief Tells if the server is ready for read or write operations.
	/// \return True if messages can be exchanged between the client and the remote server, False if no connection has been established or if the client-server handshake has not been done properly.
	bool			isReadyForOperations(void);

	/// Specifies what to do when the server authorizes socket connection.
	virtual void	serverHandshakeOkayReceived(void);

	/// Specifies what to do when the server closes the socket.
	virtual void	disconnectedFromServerReceived(void);

	/// \brief Specifies what to do upon network errors.
	/// \param	msg	Error message
	/// \deprecated	The S_ResponseMessage structure should not be used anymore. Future updates should use the global events bus system instead.
	virtual void	handleConnectionErrorsReceived(Commons::S_ResponseMessage msg);
	
	/// \brief Specifies what to do when raw datas are received.
	/// \param	data	Raw datas received from the server.
	/// \param	len		Length of the datas in bytes.
	virtual void	incomingDataReceived(const char *data, unsigned int len);

	/// \brief Processes a single Mobii packet.
	/// \param	packet	Mobii protocol-compliant packet to process.
	void			processMobiiPacket(const MobiiStandardPacket &packet);
	
	/// Specifies what to do when this client has been successfully registered as a Mobii desktop client.
	void			clientSessionOpenWithServer(void);

	/// Tells the server that this client with an ALREADY CONFIRMED UID is now active on the network.
	void			registerConfirmedClientWithServer(void);
};

#endif // COMMUNICATIONSERVICES_H_