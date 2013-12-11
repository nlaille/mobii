#ifndef AMODULEDEPENDENCYUSER_H_
#define AMODULEDEPENDENCYUSER_H_

#include		"IEventBusClient.h"
#include		"ISessionController.h"

/*! \file */

///
/// \brief Abstract class which provides a set of dependencies which may be required by the final class.
/// Such modules include the shared events bus and the session controller.
///
class			AModuleDependencyUser
{
public:
	AModuleDependencyUser(void);
	virtual ~AModuleDependencyUser(void);

	/// Sets a pointer to the global events bus.
	void		setEventBus(IEventBusClient *obj);

	/// Sets a pointer to the global user session controller.
	void		setSessionController(ISessionController *obj);
	
protected:
	/// Specifies actions to perform when the event bus is set.
	virtual void OnEventBusSet(void);

	/// Specifies actions to perform when the session controller is set.
	virtual void OnSessionControllerSet(void);

	/// Pointer to the global event bus
	IEventBusClient *	eventbus_;

	/// Pointer to the user session controller
	ISessionController *sessioncon_;
};


#endif // !AMODULEDEPENDENCYUSER_H_