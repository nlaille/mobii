#ifndef SESSIONCONTROLLER_H_
#define SESSIONCONTROLLER_H_

#include <qstring.h>

#include "ISessionController.h"
#include "IEventBusClient.h"
#include "UserEntity.h"
#include "DeviceEntity.h"
#include "ConfigurationFileIo.h"

/*! \file */

/// Holds the user and subsequent device entities used by this application modules.
class		SessionController: public ISessionController
{
public:
	explicit SessionController(IEventBusClient *eventbus);
	~SessionController(void);

	/// Loads the provided user as current user. Empty string means default user.
	virtual bool		loadUser(const QString &username);

	/// Loads the provided device only; points to the loadDevice() method from DeviceEntity.
	virtual bool		loadDevice(const QByteArray &uid);

	// Getters
	virtual UserEntity *	currentUser(void);
	virtual DeviceEntity *	currentDevice(void);

private:
	IEventBusClient *	m_eventbus;
	ConfigurationFileIo	m_cfg;

	/// Currently loaded user
	UserEntity *		m_currentUser;

	/// Currently loaded phone belonging to the current user
	DeviceEntity *		m_currentDevice;
};


#endif // !SESSIONCONTROLLER_H_