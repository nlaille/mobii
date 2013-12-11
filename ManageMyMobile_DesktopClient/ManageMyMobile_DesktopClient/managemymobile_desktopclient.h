/*
** Mobii Project -- version 0.0.1000 DEVELOPMENT VERSION
**
** Copyright(C) 2012 Mobii contributors.
**
** This library is provided `as is' and without any warranty.
** Authors decline any responsibility for any damage directly or
** indirectly caused by the use of this library.
*/

#ifndef MANAGEMYMOBILE_DESKTOPCLIENT_H
#define MANAGEMYMOBILE_DESKTOPCLIENT_H

#include <qmainwindow.h>

#include "ui_managemymobile_desktopclient.h"
#include "MobiiHomeForm.h"
#include "MobiiConnectToMobileForm.h"
#include "MobiiPhoneConnectionControlForm.h"
#include "MobiiSmsPanelForm.h"
#include "MobiiContactPanelForm.h"
#include "ui_MobiiPleaseWaitPopup.h"

#include "IBaseNetworkingEmitter.h"
#include "IBaseNetworkingReceiver.h"
#include "CommunicationServices.h"
#include "CommandsProcessor.h"
#include "netimpl.h"
#include "IEventBusClient.h"
#include "NotificationModule.h"

/*! \file */

/*! \class ManageMyMobile_DesktopClient
 *  \brief Main application class
 *
 *  This class merges all the core features of the application.
 *  It is notably in charge of displaying the main window and its content widgets.
 *  Communication services are also hosted here.
 */
class ManageMyMobile_DesktopClient : public QMainWindow
{
	Q_OBJECT

public:
	explicit ManageMyMobile_DesktopClient(IEventBusClient *eventbus, ISessionController *sessioncon);
	~ManageMyMobile_DesktopClient(void);

	/// Stores basic informations about any displayable panel in the Mobii UI.
	struct PanelProperties
	{
		/// Widget bound to this panel
		QWidget *			widget;

		/// Top bar bound button
		QAbstractButton *	switchButton;
	};

private:
	/// Qt's user interface definition
	Ui::ManageMyMobile_DesktopClientClass ui;

	/// \brief Tells if the client is already connected to the server or not.
	/// \deprecated Redundant with the isConnected() method in CommunicationServices. Future updates should use the events bus to interrogate CommunicationServices.
	bool							isConnectedToServer_;

	/// Events bus, which centralizes the application's internal communications
	IEventBusClient *				eventBus_;

	/// Session controller, which allows modules to read/write informations about the current user and his phones.
	ISessionController *			sessionController_;

	/// System tray notifications system
	NotificationModule				notificationCenter_;

	// Panels and dialogs management
	/// Provides pointers to the dependencies on which panels depend.
	void		setupModuleDependencies(AModuleDependencyUser *module);

	/// Stores a list of all panels currently managed by the Mobii UI.
	QMap<QString, PanelProperties>	panelSet_;

	/// Initializes all panels in the UI.
	void		initializePanelSet(void);

	/// Adds an initialized panel object to the list and associate a name and a button which will display it.
	void		addToPanelSet(const char *panelName, QAbstractButton *switchButton, QWidget *widget);

	/// Hides the current panel and displays the given panel instead.
	void		switchActivePanel(const QString &panelName);

	/// Stores a list of all dialogs currently managed by the Mobii UI.
	QMap<QString, QDialog *>		dialogSet_;

	/// Opens a dialog identified by the given name.
	void		openDialogForm(const QString &name);

	/// Initializes all dialogs in the UI.
	void		initializeDialogSet(void);

	/// Adds an initialized dialog object to the list and associate a new.
	void		addToDialogSet(const char *dialogName, QDialog *widget);

	/// Handles state changes on the window itself
	virtual void changeEvent(QEvent *e);

	/// Handles window close requests.
	virtual void closeEvent(QCloseEvent *e);

private slots:
	// User input slots
	void	User_DisconnectSession(void);
	void	User_OpenLegalNoticeForm(void);
	void	User_OpenConfigurationForm(void);
	void	User_ChangeMainPanel(void);

	// UI slots
	void	UI_ChangeMainPanel(const QString eventName, void *sender, void *data);
	void	UI_ServerErrorReport(const QString eventName, void *sender, void *data);
	void	UI_ConnectingToServer(const QString eventName, void *sender, void *data);
	void	UI_ConnectionFailedMissingField(const QString eventName, void *sender, void *data);
	void	UI_ServerConnected(const QString eventName, void *sender, void *data);
	void	UI_ServerDisconnected(const QString eventName, void *sender, void *data);
	void	UI_ServerError(const QString eventName, void *sender, void *data);
	void	UI_UserLoginFailed(const QString eventName, void *sender, void *data);
	void	UI_MobileHasRefused(const QString eventName, void *sender, void *data);
	void	UI_MobileHasAccepted(const QString eventName, void *sender, void *data);
	void	UI_BlockingOperation(const QString eventName, void *sender, void *data);
	void	UI_MobileWasDisconnected(const QString eventName, void *sender, void *data);
	void	UI_UidConfirmatedFailed(const QString eventName, void *sender, void *data);
	void	UI_HideUnavailableFeatures(const QString eventName, void *sender, void *data);
	void	UI_WindowManipulation(const QString eventName, void *sender, void *data);

	// Misc.
	void	UI_MinimizeInSystray(void);
};

#endif // MANAGEMYMOBILE_DESKTOPCLIENT_H
