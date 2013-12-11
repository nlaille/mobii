#ifndef IEVENTBUSCLIENT_H_
#define IEVENTBUSCLIENT_H_

#include		<qobject.h>
#include		<qstring.h>

/*! \file */

///
/// \brief Event bus module public interface, for use by publishers and subscribers.
///
class IEventBusClient
{
public:
	virtual ~IEventBusClient(void) {}

	/// Registers the sending object as a consumer of `eventName' events.
	virtual void	registerAsConsumer(const QString &eventName, const QObject *me, const char *slot) = 0;

	/// Unregisters all connections for this consumer. Useful when `me' is destroyed.
	virtual void	unregisterConsumer(const QObject *me) = 0;

	/// Triggers an event which will be interpreted by receivers.
	virtual void	emitEvent(const QString &eventName, void* sender, void *data) = 0;

	/// Convenient version of the fuller version of this method, with sender and data set to NULL
	virtual void	emitEvent(const QString &eventName) = 0;
};


#endif // !IEVENTBUSCLIENT_H_