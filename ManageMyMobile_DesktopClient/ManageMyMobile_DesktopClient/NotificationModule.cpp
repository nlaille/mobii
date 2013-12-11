#include		<qvariant.h>
#include		<qtimer.h>

#include		"MobiiEventBusCodes.h"
#include		"NotificationModule.h"


NotificationModule::NotificationModule(void)
{
	// Systray icon prepare
	QIcon *icon = new QIcon(":/common/Resources/logo_small.png");
	this->m_systray.setIcon(*icon);
	this->m_systray.setToolTip("Mobii");
	this->m_systray.show();

	// Context menu prepare
	this->m_menu = new QMenu();
	QAction *restoreAction = this->m_menu->addAction("Ouvrir Mobii", this, SLOT(OnMenuRestoreClicked(bool)));
	this->m_menu->addSeparator();
	QAction *quitAction = this->m_menu->addAction("Quitter", this, SLOT(OnMenuQuitClicked(bool)));
	this->m_systray.setContextMenu(this->m_menu);

	connect(&(this->m_systray), SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(OnSystrayIconActivated(QSystemTrayIcon::ActivationReason)));
	connect(&(this->m_systray), SIGNAL(messageClicked()), this, SLOT(OnMessageBalloonClicked()));
}


NotificationModule::~NotificationModule(void)
{
	this->m_systray.hide();
}


void		NotificationModule::OnEventBusSet(void)
{
	this->eventbus_->registerAsConsumer(DISPLAY_NOTIFICATION_SYSTRAY, this, SLOT(OnNotificationRequest(const QString, void *, void *)));
}


void		NotificationModule::OnNotificationRequest(const QString eventName, void *sender, void *data)
{
	QMap<QString, QString> args = *((QMap<QString, QString>*)data);
	QString title = (args.contains("title") ? args["title"] : "Information");
	QString msg = (args.contains("msg") ? args["msg"] : "");
	QSystemTrayIcon::MessageIcon icon = QSystemTrayIcon::NoIcon;
	if (args.contains("icon"))
	{
		if (args["icon"] == "warn")
			icon = QSystemTrayIcon::Warning;
		else if (args["icon"] == "error")
			icon = QSystemTrayIcon::Critical;
		else if (args["icon"] == "info")
			icon = QSystemTrayIcon::Information;
	}
	if (args.contains("emitOnClick"))
	{
		this->m_emitWhenBalloonClicked = args["emitOnClick"];
		this->m_emitWhenBalloonClicked_data = args["emitOnClickData"];
	}
	else
		this->m_emitWhenBalloonClicked.clear();
	this->m_systray.showMessage(title, msg, icon, (args.contains("timeout") ? args["timeout"].toInt() : 10000));
}


void		NotificationModule::OnSystrayIconActivated(QSystemTrayIcon::ActivationReason reason)
{
	if (reason == QSystemTrayIcon::DoubleClick)
		this->eventbus_->emitEvent(WINDOW_RESTORE);
}


void		NotificationModule::OnMenuQuitClicked(bool checked)
{
	this->eventbus_->emitEvent(APP_CLOSE);
}


void		NotificationModule::OnMenuRestoreClicked(bool checked)
{
	this->eventbus_->emitEvent(WINDOW_RESTORE);
}


void		NotificationModule::OnMessageBalloonClicked(void)
{
	if (this->m_emitWhenBalloonClicked != "")
		this->eventbus_->emitEvent(this->m_emitWhenBalloonClicked, this, (void *)&(this->m_emitWhenBalloonClicked_data));
}