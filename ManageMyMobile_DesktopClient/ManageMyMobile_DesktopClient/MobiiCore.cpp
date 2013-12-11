#include		<qapplication.h>
#include		<qdir.h>
#include		<qvector.h>
#include		<qstandardpaths.h>

#include		"MobiiCore.h"


MobiiCore::MobiiCore(void)
{
	this->eventBus_ = new EventBus();
	this->sessionController_ = new SessionController(this->eventBus_);
	this->netbase_ = new NetImpl();
	this->commServices_ = new CommunicationServices(this->netbase_);
	this->commServices_->setEventBus(this->eventBus_);
	this->commServices_->setSessionController(this->sessionController_);
	this->netbase_->bindEventsToReceiverClass(this->commServices_);

	// Setup default user
	this->sessionController_->loadUser();

	// Setup main components
	this->mainWindow_ = new ManageMyMobile_DesktopClient(this->eventBus_, this->sessionController_);
}


MobiiCore::~MobiiCore(void)
{
	delete this->mainWindow_;
	delete this->eventBus_;
	delete this->sessionController_;
}


void			MobiiCore::start(void)
{
	// Setup events listeners
	this->eventBus_->registerAsConsumer(CORE_REQ_RESETUID, this, SLOT(OnCoreRequestReceived(const QString, void *, void *)));
	this->eventBus_->registerAsConsumer(CORE_REQ_RESETUSERSTOR, this, SLOT(OnCoreRequestReceived(const QString, void *, void *)));
	this->eventBus_->registerAsConsumer(CORE_REQ_RESETSRVSETTINGS, this, SLOT(OnCoreRequestReceived(const QString, void *, void *)));
	
		// Startup actions
	this->OnApplicationStart();

	// Display the main window
	this->mainWindow_->show();
}


void			MobiiCore::setupApplicationHandle(QApplication &app)
{
	app.setApplicationName("Mobii");
}


void			MobiiCore::OnApplicationStart(void)
{
	this->setupUid();
	this->setupUserStorage();
	this->configureWatchedEvents();
}


void			MobiiCore::OnCoreRequestReceived(const QString eventName, void* sender, void *data)
{
	QString answer = "ANSWER_" + eventName;
	if (eventName == CORE_REQ_RESETUID)
		this->setupUid(true);
	else if (eventName == CORE_REQ_RESETUSERSTOR)
		this->setupUserStorage(true);
	else if (eventName == CORE_REQ_RESETSRVSETTINGS)
		this->setupDefaultServerSettings();
	else
		qDebug() << Q_FUNC_INFO << "event not handled:" << eventName;
}


void			MobiiCore::configureWatchedEvents(void)
{
	this->eventBus_->registerAsConsumer(CORE_REQ_RESETUID, this, SLOT(OnCoreRequestReceived(const QString, void* , void *)));
	this->eventBus_->registerAsConsumer(CORE_REQ_RESETUSERSTOR, this, SLOT(OnCoreRequestReceived(const QString, void* , void *)));
}


void			MobiiCore::setupUid(bool forceReset)
{
	if (this->coreConfig_.clientUid().isEmpty() || forceReset == true)
	{
		// Generate seed
		QTime time = QTime::currentTime();
		qsrand((uint)time.msec());

		// Randomize UID
		QByteArray uid;
		qDebug() << "Client UUID not defined. Self-generating the UUID.";
		for (int i = 0; i < 6; ++i)
		{
			int val = 0;
			while ((val = qrand() % 255) == 0);
			uid.append(val);
		}
		this->coreConfig_.setClientUid(uid);
	}
}


void			MobiiCore::setupUserStorage(bool forceReset)
{
	QString		pathAsString = this->coreConfig_.userStorageDir();
	QDir		userDir;

	if (pathAsString.isEmpty() || !QDir(pathAsString).exists() || forceReset == true)
	{
		// Generate a list of possible locations which could serve as storage places
		QVector<QString> locationCandidates;
		locationCandidates.push_back(QStandardPaths::writableLocation(QStandardPaths::DataLocation));
		locationCandidates.push_back(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation));
		locationCandidates.push_back(QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation));
		locationCandidates.push_back(QStandardPaths::writableLocation(QStandardPaths::HomeLocation));
		locationCandidates.push_back(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation));

		// Select the first available candidate
		QString selectedCandidate;
		for (QVector<QString>::iterator it = locationCandidates.begin(); it != locationCandidates.end(); ++it)
		{
			selectedCandidate = *it;
			if (!selectedCandidate.isEmpty() && QDir().mkpath(selectedCandidate))
				break;
		}
		if (selectedCandidate.isEmpty())
			qDebug() << Q_FUNC_INFO << "Unable to locate a suitable storage location by default.";
		else
			qDebug() << "Default storage location is now:" << selectedCandidate;
		QDir().mkpath(selectedCandidate);
		this->coreConfig_.setUserStorageDir(selectedCandidate);
	}
}


void			MobiiCore::setupDefaultServerSettings(void)
{
	this->coreConfig_.setServerAddress("127.0.0.1");
	this->coreConfig_.setServerTcpPort(3689);
	this->coreConfig_.setDirectConnectionPort(53789);
	this->coreConfig_.setUseSsl(true);
}