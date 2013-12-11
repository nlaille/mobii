#ifndef MOBIICORE_H_
#define MOBIICORE_H_

#include	<qobject.h>

#include	"CommunicationServices.h"
#include	"MobiiEventBusCodes.h"
#include	"ConfigurationFileIo.h"
#include	"managemymobile_desktopclient.h"
#include	"EventBus.h"
#include	"SessionController.h"
#include	"netimpl.h"

/*! \file */

///
/// \brief Hosts all Mobii major modules.
///
class		MobiiCore: public QObject
{
	Q_OBJECT

public:
	MobiiCore(void);
	~MobiiCore(void);

	/// Displays the starting window
	void	start(void);

	/// Configures some application parameters
	void	setupApplicationHandle(QApplication &);

private slots:
	/// Processes action when another module makes a request to this core module.
	void	OnCoreRequestReceived(const QString eventName, void* sender, void *data);

private:
	/// Prepares a new unique Mobii identifier upon first launch.
	void	setupUid(bool forceReset = false);

	/// Resets the user datas storage directory to its default value.
	void	setupUserStorage(bool forceReset = false);

	/// Resets all server settings to their default values.
	void	setupDefaultServerSettings(void);

	/// Internal system actions to perform upon application startup
	void	OnApplicationStart(void);

	/// Registers this super-module as consumer for all requests such as setup a new UID, or reset client settings to defaults.
	void	configureWatchedEvents(void);

	/// Mobii main configuration holder
	ConfigurationFileIo				coreConfig_;

	/// Main window, which is is notably in charge of displaying the user interface.
	ManageMyMobile_DesktopClient *	mainWindow_;

	/// Network communication foundation
	CommunicationServices *			commServices_;

	/// Network low-level module
	NetImpl *						netbase_;

	/// Events bus, which centralizes the application's internal communications
	IEventBusClient *				eventBus_;

	/// Session controller, which allows modules to read/write informations about the current user and his phones.
	ISessionController *			sessionController_;
};


#endif // !MOBIICORE_H_