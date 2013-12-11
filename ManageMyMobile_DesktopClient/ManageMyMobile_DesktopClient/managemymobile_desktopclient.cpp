/*
** Mobii Project -- version 0.0.1000 DEVELOPMENT VERSION
**
** Copyright(C) 2012 Mobii contributors.
**
** This library is provided `as is' and without any warranty.
** Authors decline any responsibility for any damage directly or
** indirectly caused by the use of this library.
*/

#ifdef C_PLATFORM_TYPE_WINDOWS
#include <Windows.h>
#endif

#include <iostream>
#include <stdexcept>

#include <qdir.h>
#include <qsettings.h>
#include <qmessagebox.h>
#include <qtimer.h>

#include "commons.h"
#include "netimpl.h"
#include "EventBus.h"
#include "MobiiEventBusCodes.h"
#include "CommunicationServices.h"
#include "managemymobile_desktopclient.h"
#include "AExtendedUiWidget.h"
#include "NotificationModule.h"

#include "MobiiImagePanel.h"
#include "MobiiConfigurationDialog.h"
#include "MobiiLegalNoticeDialog.h"
#include "MobiiCalendarPanel.h"

ManageMyMobile_DesktopClient::ManageMyMobile_DesktopClient(IEventBusClient *eventbus, ISessionController *sessioncon)
	: eventBus_(eventbus), sessionController_(sessioncon), isConnectedToServer_(false)
{
	ui.setupUi(this);

	// Initialize panels
	this->initializePanelSet();
	this->initializeDialogSet();

	// Initialize startup-time modules
	this->setupModuleDependencies(&(this->notificationCenter_));

	// Slots binding
	// -- User inputs
	connect(this->ui.DisconnectLink, SIGNAL(clicked()), this, SLOT(User_DisconnectSession()));
	connect(this->ui.TopSettingsButton, SIGNAL(clicked()), this, SLOT(User_OpenConfigurationForm()));
	connect(this->ui.TopLegalNoticeButton, SIGNAL(clicked()), this, SLOT(User_OpenLegalNoticeForm()));
	// -- Top menu
	connect(this->ui.TopHomeButton, SIGNAL(clicked()), this, SLOT(User_ChangeMainPanel()));
	connect(this->ui.TopNewConnectionButton, SIGNAL(clicked()), this, SLOT(User_ChangeMainPanel()));
	connect(this->ui.TopMobileInfosPanelButton, SIGNAL(clicked()), this, SLOT(User_ChangeMainPanel()));
	connect(this->ui.TopSmsPanelButton, SIGNAL(clicked()), this, SLOT(User_ChangeMainPanel()));
	connect(this->ui.TopContactPanelButton, SIGNAL(clicked()), this, SLOT(User_ChangeMainPanel()));
	connect(this->ui.TopCalendarPanelButton, SIGNAL(clicked()), this, SLOT(User_ChangeMainPanel()));
	connect(this->ui.TopImagePanelButton, SIGNAL(clicked()), this, SLOT(User_ChangeMainPanel()));

	// -- Events bus registrations
	this->eventBus_->registerAsConsumer(SERVER_ERROR_REPORT, this, SLOT(UI_ServerErrorReport(const QString, void *, void *)));
	this->eventBus_->registerAsConsumer(SERVER_SETTINGS_NOT_SET, this, SLOT(UI_ConnectionFailedMissingField(const QString, void *, void *)));
	this->eventBus_->registerAsConsumer(NET_SERVER_CONNECTED, this, SLOT(UI_ServerConnected(const QString, void *, void *)));
	this->eventBus_->registerAsConsumer(NET_SERVER_DISCONNECTED, this, SLOT(UI_ServerDisconnected(const QString, void *, void *)));
	this->eventBus_->registerAsConsumer(NET_SERVER_ERROR, this, SLOT(UI_ServerError(const QString, void *, void *)));
	this->eventBus_->registerAsConsumer(USER_LOGIN_FAILED, this, SLOT(UI_UserLoginFailed(const QString, void *, void *)));
	this->eventBus_->registerAsConsumer(BLOCKING_OPERATION_BEGIN, this, SLOT(UI_BlockingOperation(const QString, void *, void *)));
	this->eventBus_->registerAsConsumer(BLOCKING_OPERATION_END, this, SLOT(UI_BlockingOperation(const QString, void *, void *)));
	this->eventBus_->registerAsConsumer(BINDING_ATTEMPT_OKAY, this, SLOT(UI_MobileHasAccepted(const QString, void *, void *)));
	this->eventBus_->registerAsConsumer(BINDING_ATTEMPT_FAILED, this, SLOT(UI_MobileHasRefused(const QString, void *, void *)));
	this->eventBus_->registerAsConsumer(NET_MOBILE_DISCONNECTED, this, SLOT(UI_MobileWasDisconnected(const QString, void *, void *)));
	this->eventBus_->registerAsConsumer(CONNECT_TO_SERVER, this, SLOT(UI_ConnectingToServer(const QString, void *, void *)));
	this->eventBus_->registerAsConsumer(UID_REGISTRATION_FAILED, this, SLOT(UI_UidConfirmatedFailed(const QString, void *, void *)));
	this->eventBus_->registerAsConsumer(FEATURES_AVAILABLE_ON_PHONE, this, SLOT(UI_HideUnavailableFeatures(const QString, void *, void *)));
	this->eventBus_->registerAsConsumer(WINDOW_RESTORE, this, SLOT(UI_WindowManipulation(const QString, void *, void *)));
	this->eventBus_->registerAsConsumer(APP_CLOSE, this, SLOT(UI_WindowManipulation(const QString, void *, void *)));
	this->eventBus_->registerAsConsumer(UI_CHANGE_ACTIVE_PANEL, this, SLOT(UI_ChangeMainPanel(const QString, void *, void *)));

	// Tweaks before starting
	this->ui.TopHomeButton->click();
	this->ui.TopConnectionEstablishedMenu->setVisible(false);
	this->ui.DisconnectLink->setVisible(false);
}

void		ManageMyMobile_DesktopClient::changeEvent(QEvent *e)
{
	if (e->type() == QEvent::WindowStateChange)
	{
		if (this->windowState() & Qt::WindowMinimized)
			this->UI_MinimizeInSystray();
	}
	QMainWindow::changeEvent(e);
}

void		ManageMyMobile_DesktopClient::closeEvent(QCloseEvent *e)
{
	this->UI_MinimizeInSystray();
	e->ignore();
}

void		ManageMyMobile_DesktopClient::UI_MinimizeInSystray(void)
{
	// Hide the taskbar icon as a system tray icon is already there
	QTimer::singleShot(0, this, SLOT(hide()));

	// Display notification bullet
	static bool showOnce = true;
	if (showOnce == true)
	{
		QMap<QString, QString> args;
		args["title"] = "Mobii est encore actif";
		args["msg"] = "Mobii continuera de recevoir les notification de votre téléphone, si vous êtes connecté.";
		args["icon"] = "info";
		showOnce = false;
		this->eventBus_->emitEvent(DISPLAY_NOTIFICATION_SYSTRAY, this, &args);
	}
}

void		ManageMyMobile_DesktopClient::initializePanelSet(void)
{
	this->addToPanelSet("TopContactPanelButton", this->ui.TopContactPanelButton, new MobiiContactPanelForm());
	this->addToPanelSet("TopHomeButton", this->ui.TopHomeButton, new MobiiHomeForm());
	this->addToPanelSet("TopNewConnectionButton", this->ui.TopNewConnectionButton, new MobiiConnectToMobileForm());
	this->addToPanelSet("TopMobileInfosPanelButton", this->ui.TopMobileInfosPanelButton, new MobiiPhoneConnectionControlForm());
	this->addToPanelSet("TopSmsPanelButton", this->ui.TopSmsPanelButton, new MobiiSmsPanelForm());
	this->addToPanelSet("TopCalendarPanelButton", this->ui.TopCalendarPanelButton, new MobiiCalendarPanel());
	this->addToPanelSet("TopImagePanelButton", this->ui.TopImagePanelButton, new MobiiFilePanel());
}

void		ManageMyMobile_DesktopClient::initializeDialogSet(void)
{
	this->addToDialogSet("ConfigDialog", new MobiiConfigurationDialog());
	this->addToDialogSet("LegalDialog", new MobiiLegalNoticeDialog());
}

void		ManageMyMobile_DesktopClient::setupModuleDependencies(AModuleDependencyUser *module)
{
	module->setEventBus(this->eventBus_);
	module->setSessionController(this->sessionController_);
}

void		ManageMyMobile_DesktopClient::addToPanelSet(
	const char *panelName, 
	QAbstractButton *switchButton, 
	QWidget *widget)
{
	if (dynamic_cast<AModuleDependencyUser *>(widget) != NULL)
		this->setupModuleDependencies(dynamic_cast<AModuleDependencyUser *>(widget));
	else
		qDebug() << Q_FUNC_INFO << "panel with identifier `" << panelName << "' is not a valid AModuleDependencyUser";

	widget->setStyleSheet(UI_DEFAULT_STYLE);
	PanelProperties &props = this->panelSet_[panelName];
	switchButton->setCursor(Qt::PointingHandCursor);
	switchButton->setAutoExclusive(false);
	props.switchButton = switchButton;
	props.widget = widget;
	this->ui.ContentWidgetStack->addWidget(widget);
}

void		ManageMyMobile_DesktopClient::addToDialogSet(
	const char *dialogName, 
	QDialog *widget)
{
	if (dynamic_cast<AModuleDependencyUser *>(widget) != NULL)
		this->setupModuleDependencies(dynamic_cast<AModuleDependencyUser *>(widget));
	else
		qDebug() << Q_FUNC_INFO << "dialog with identifier `" << dialogName << "' is not a valid AModuleDependencyUser";

	widget->setStyleSheet(UI_DEFAULT_STYLE);
	widget->setWindowModality(Qt::ApplicationModal);
	this->dialogSet_[dialogName] = widget;
}

ManageMyMobile_DesktopClient::~ManageMyMobile_DesktopClient(void)
{
}

/************************
** SLOTS IMPLEMENTATIONS
************************/

void		ManageMyMobile_DesktopClient::User_ChangeMainPanel(void)
{
	QObject *sender = QObject::sender();
	this->switchActivePanel(sender->objectName());
}

void		ManageMyMobile_DesktopClient::UI_ChangeMainPanel(const QString eventName, void *sender, void *data)
{
	QString *panelName = (QString *)data;
	this->switchActivePanel(*panelName);
}

void		ManageMyMobile_DesktopClient::switchActivePanel(const QString &panelName)
{
	QMap<QString, PanelProperties>::iterator it = this->panelSet_.find(panelName);
	if (it != this->panelSet_.end())
	{
		// Notify the previous panel that it is going to be hidden, if possible
		AExtendedUiWidget *old = dynamic_cast<AExtendedUiWidget *>(this->ui.ContentWidgetStack->currentWidget());
		if (old != NULL)
			old->OnWidgetClose();
		for (QMap<QString, PanelProperties>::iterator it2 = this->panelSet_.begin(); it2 != this->panelSet_.end(); ++it2)
		{
			(*it2).switchButton->setChecked(false);
			(*it2).switchButton->setStyleSheet((*it2).switchButton->styleSheet());
		}

		// Notify the new panel that it is going to be shown
		AExtendedUiWidget *widget = dynamic_cast<AExtendedUiWidget *>((*it).widget);
		if (widget != NULL)
			widget->OnWidgetShow();
		it.value().switchButton->setChecked(true);
		it.value().switchButton->setStyleSheet(it.value().switchButton->styleSheet());
		this->ui.ContentWidgetStack->setCurrentWidget(it.value().widget);
	}
	else
		qDebug() << "switchActivePanel(): panel not found: " << panelName;
}

void		ManageMyMobile_DesktopClient::openDialogForm(const QString &name)
{
	QMap<QString, QDialog *>::iterator it = this->dialogSet_.find(name);
	if (it != this->dialogSet_.end())
	{
		AExtendedUiWidget *widget = dynamic_cast<AExtendedUiWidget *>(*it);
		if (widget != NULL)
			widget->OnWidgetShow();
		(*it)->exec();
	}
	else
		qDebug() << Q_FUNC_INFO << "dialog not found:" << name;
}

void		ManageMyMobile_DesktopClient::User_DisconnectSession(void)
{
	this->eventBus_->emitEvent(DISCONNECT_FROM_SERVER, this, NULL);
}

void		ManageMyMobile_DesktopClient::User_OpenConfigurationForm(void)
{
	this->openDialogForm("ConfigDialog");
}

void		ManageMyMobile_DesktopClient::User_OpenLegalNoticeForm(void)
{
	this->openDialogForm("LegalDialog");
}


/**************************
** UI SLOTS IMPLEMENTATIONS
**************************/

void		ManageMyMobile_DesktopClient::UI_ServerErrorReport(const QString eventName, void *sender, void *data)
{
	this->eventBus_->emitEvent(BLOCKING_OPERATION_END, this, NULL);
	this->eventBus_->emitEvent(DISCONNECT_FROM_SERVER, this, NULL);
	QMessageBox mbox(
		QMessageBox::Warning, 
		"Erreur serveur",
		"Le serveur Mobii a rapporté une erreur et a été déconnecté.");
	mbox.exec();
	qDebug() << "Server error notified.";
}

void		ManageMyMobile_DesktopClient::UI_ConnectingToServer(const QString eventName, void *sender, void *data)
{
	this->ui.MainInfoLabel->setText("Veuillez patienter, connexion en cours...");
}

void		ManageMyMobile_DesktopClient::UI_ConnectionFailedMissingField(const QString eventName, void *sender, void *data)
{
	QMessageBox mbox(QMessageBox::Critical, "Connexion impossible", "Certains paramètres de connexion au serveur ne sont pas renseignés.\nLa fenêtre de configuration de Mobii va s'ouvrir pour que vous puissiez le faire.");
	mbox.exec();
	this->UI_ServerDisconnected("", NULL, NULL);
	this->openDialogForm("ConfigDialog");
}

void		ManageMyMobile_DesktopClient::UI_ServerDisconnected(const QString eventName, void *sender, void *data)
{
	this->isConnectedToServer_ = false;
	this->ui.MainInfoLabel->setText("Vous n'êtes pas connecté au serveur Mobii.");
	this->ui.centralWidget->setEnabled(true);
	this->setCursor(QCursor(Qt::ArrowCursor));
	this->ui.TopInfosBar->setStyleSheet("#TopInfosBar { background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 rgb(255, 55, 55), stop: 1 #e0eff9);}");
	this->ui.TopNewConnectionButton->setVisible(true);
	this->ui.TopConnectionEstablishedMenu->setVisible(false);
	this->ui.TopHomeButton->setVisible(true);
	this->switchActivePanel("TopHomeButton");
	this->ui.DisconnectLink->setVisible(false);
	this->eventBus_->emitEvent(BLOCKING_OPERATION_END, this, NULL);
}

void		ManageMyMobile_DesktopClient::UI_ServerConnected(const QString eventName, void *sender, void *data)
{
	QString upperBarTitle = "Vous êtes connecté";
	if (this->sessionController_->currentUser()->username() != USERENTITY_DEFAULT)
		upperBarTitle.append(", " + this->sessionController_->currentUser()->username());
	this->ui.MainInfoLabel->setText(upperBarTitle);
	this->isConnectedToServer_ = true;
	this->ui.centralWidget->setEnabled(true);
	this->setCursor(QCursor(Qt::ArrowCursor));
	this->ui.TopInfosBar->setStyleSheet("");
	this->ui.TopHomeButton->setVisible(false);
	this->ui.DisconnectLink->setVisible(true);
	this->switchActivePanel("TopNewConnectionButton");
	this->eventBus_->emitEvent(BLOCKING_OPERATION_END, this, NULL);
}

void		ManageMyMobile_DesktopClient::UI_ServerError(const QString eventName, void *sender, void *data)
{
	if (isConnectedToServer_ == false)
		this->eventBus_->emitEvent(NET_SERVER_DISCONNECTED, this, NULL);
	Commons::S_ResponseMessage *msg = (Commons::S_ResponseMessage *)(data);
	QMessageBox mbox(
		QMessageBox::Critical, 
		"Erreur reseau",
		QString("Une erreur liée au réseau est survenue : ") + msg->message.c_str());
	mbox.exec();
	this->eventBus_->emitEvent(BLOCKING_OPERATION_END, this, NULL);
}

void		ManageMyMobile_DesktopClient::UI_UserLoginFailed(const QString eventName, void *sender, void *data)
{
	if (isConnectedToServer_ == false)
		this->eventBus_->emitEvent(NET_SERVER_DISCONNECTED, this, NULL);
	QMessageBox mbox(
		QMessageBox::Warning, 
		"Echec de l'authentification",
		"Le nom d'utilisateur ou le mot de passe que vous avez entré est incorrect. Vérifiez ces informations puis réessayez.");
	mbox.exec();
	this->eventBus_->emitEvent(BLOCKING_OPERATION_END, this, NULL);
}

void		ManageMyMobile_DesktopClient::UI_BlockingOperation(const QString eventName, void *sender, void *data)
{
	if (eventName == BLOCKING_OPERATION_BEGIN)
	{
		this->ui.centralWidget->setEnabled(false);
		this->setCursor(Qt::WaitCursor);
	}
	else
	{
		this->ui.centralWidget->setEnabled(true);
		this->setCursor(Qt::ArrowCursor);
	}
}

void		ManageMyMobile_DesktopClient::UI_MobileHasAccepted(const QString eventName, void *sender, void *data)
{
	this->ui.TopConnectionEstablishedMenu->setVisible(true);
	this->ui.TopNewConnectionButton->setVisible(false);
	this->switchActivePanel("TopMobileInfosPanelButton");

	QMap<QString, QString> args;
	args["title"] = "Connexion établie";
	args["msg"] = "Vous êtes maintenant connecté à votre téléphone.";
	args["icon"] = "info";
	this->eventBus_->emitEvent(DISPLAY_NOTIFICATION_SYSTRAY, this, &args);
}

void		ManageMyMobile_DesktopClient::UI_MobileHasRefused(const QString eventName, void *sender, void *data)
{
	QMessageBox mbox(
		QMessageBox::Warning, 
		"Impossible de se connecter au mobile",
		QString("Le téléphone distant a refusé votre demande de connexion."));
	mbox.exec();
}

void		ManageMyMobile_DesktopClient::UI_MobileWasDisconnected(const QString eventName, void *sender, void *data)
{
	this->ui.TopNewConnectionButton->setVisible(true);
	this->ui.TopConnectionEstablishedMenu->setVisible(false);
	this->switchActivePanel("TopHomeButton");
	QMessageBox mbox(
		QMessageBox::Warning, 
		"Connexion avec le téléphone interrompue",
		QString("Le téléphone distant n'est plus connecté au serveur."));
	mbox.exec();
}

void		ManageMyMobile_DesktopClient::UI_UidConfirmatedFailed(const QString eventName, void *sender, void *data)
{
	QMessageBox mbox(
		QMessageBox::Critical, 
		"Erreur d'autorisation serveur",
		QString("Le serveur a refusé l'identifiant unique de votre client. Veuillez retenter de vous connecter.\nSi l'erreur persiste, merce de contacter le support Mobii.\nCode erreur: ") + UID_REGISTRATION_FAILED);
	mbox.exec();
}

void		ManageMyMobile_DesktopClient::UI_HideUnavailableFeatures(const QString eventName, void *sender, void *data)
{
	QVector<QString> &res(*((QVector<QString> *)data));
	bool allfeatures = res.isEmpty();
	this->ui.TopSmsPanelButton->setVisible(res.contains("sms") || allfeatures);
	this->ui.TopContactPanelButton->setVisible(res.contains("contacts") || allfeatures);
	this->ui.TopCalendarPanelButton->setVisible(res.contains("calendar") || allfeatures);
	this->ui.TopImagePanelButton->setVisible(res.contains("files") || allfeatures);
}

void		ManageMyMobile_DesktopClient::UI_WindowManipulation(const QString eventName, void *sender, void *data)
{
	if (eventName == WINDOW_RESTORE)
	{
		this->raise();
		this->activateWindow();
		this->showNormal();
	}
	else if (eventName == APP_CLOSE)
	{
		qApp->quit();
	}
}