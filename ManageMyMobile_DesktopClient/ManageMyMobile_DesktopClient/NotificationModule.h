#ifndef NOTIFICATIONMODULE_H_
#define NOTIFICATIONMODULE_H_

#include		<qobject.h>
#include		<QSystemTrayIcon>
#include		<qmenu.h>

#include		"AModuleDependencyUser.h"

/*! \file */

/**
 *	\brief Provides a simple common interface to display notifications to the user.
 *
 *	Structure of datas to send to DISPLAY_NOTIFICATION_SYSTRAY:
 *
 *	QMap<QString, QString>
 *	-	"title"		=>	title of the message which will be displayed in the notification balloon
 *	-	"msg"		=>	body of the message (may be truncated if too long)
 *	-	"icon"		=>	illustration icon (platform-dependant)
 *						choices are: warn / error / info / (none if the "icon" key is not defined)
 *	-	"timeout"	=>	time after which the message is automatically dismissed (platform-dependant)
 *						must be specified as milliseconds (1s = 1,000ms)
 */
class			NotificationModule : public QObject, public AModuleDependencyUser
{
	Q_OBJECT

public:
	NotificationModule(void);
	~NotificationModule(void);

private:
	/// Interface which offers a systray icon with a notifications system
	QSystemTrayIcon		m_systray;

	/// Context menu shown when user right-clicks on the system tray icon
	QMenu *				m_menu;

	/// Holds the event to emit when user clicks on a balloon
	QString				m_emitWhenBalloonClicked;

	/// Holds the datas associated with the event sent when cliclink on a balloon.
	QString				m_emitWhenBalloonClicked_data;

	virtual void		OnEventBusSet(void);

private slots:
	/// Processes a notification display request from a module
	void		OnNotificationRequest(const QString eventName, void *sender, void *data);

	/// Handles mouse clicks on the systray icon
	void		OnSystrayIconActivated(QSystemTrayIcon::ActivationReason reason);

	/// Quits the application when clicking on Quit from the systray context menu.
	void		OnMenuQuitClicked(bool checked);

	/// Displays the application on foreground when double-clicking on the systray icon or selecting the appropriate option in the context menu.
	void		OnMenuRestoreClicked(bool checked);

	/// Handles click on a balloon
	void		OnMessageBalloonClicked(void);
};


#endif // !NOTIFICATIONMODULE_H_