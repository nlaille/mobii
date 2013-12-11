#include	<exception>
#include	<stdexcept>

#include	"SessionController.h"


SessionController::SessionController(IEventBusClient *eventbus)
	: m_eventbus(eventbus), m_currentUser(NULL), m_currentDevice(NULL)
{
}


SessionController::~SessionController(void)
{
}


bool			SessionController::loadUser(const QString &username)
{
	if (this->m_currentUser != NULL)
		delete this->m_currentUser;
	if (username == "")
		this->m_currentUser = UserEntity::loadDefaultUser(this->m_cfg.userStorageDir());
	else
		this->m_currentUser = UserEntity::loadUser(username, this->m_cfg.userStorageDir());
	return this->m_currentUser != NULL;
}


bool			SessionController::loadDevice(const QByteArray &uid)
{
	if (this->m_currentUser == NULL)
	{
		qDebug() << Q_FUNC_INFO << "user not loaded.";
		return false;
	}
	try {
		this->m_currentDevice = this->m_currentUser->deviceByUid(uid);
	} catch (const std::exception &e) {
		qDebug() << Q_FUNC_INFO << "error notified:" << e.what();
		return false;
	}
	return true;
}


UserEntity *	SessionController::currentUser(void)
{
	if (this->m_currentUser == NULL)
	{
		qDebug() << Q_FUNC_INFO << "SessionController: no user loaded.";
		return NULL;
	}
	return this->m_currentUser;
}


DeviceEntity *	SessionController::currentDevice(void)
{
	if (this->m_currentDevice == NULL)
	{
		qDebug() << Q_FUNC_INFO << "SessionController: no device loaded.";
		return NULL;
	}
	return this->m_currentDevice;
}