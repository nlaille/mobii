#ifndef ISESSIONCONTROLLER_H_
#define ISESSIONCONTROLLER_H_

#include	<qstring.h>

#include	"UserEntity.h"

/*! \file */

///
/// \brief Public interface for the user session controller (SessionController).
///
class		ISessionController
{
public:
	virtual ~ISessionController(void) {}

	virtual bool			loadUser(const QString &username = "") = 0;
	virtual bool			loadDevice(const QByteArray &uid) = 0;

	virtual UserEntity *	currentUser(void) = 0;
	virtual DeviceEntity *	currentDevice(void) = 0;
};


#endif // !ISESSIONCONTROLLER_H_