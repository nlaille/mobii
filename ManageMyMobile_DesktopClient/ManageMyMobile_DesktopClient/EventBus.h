#ifndef EVENTBUS_H_
#define EVENTBUS_H_

#include <qobject.h>
#include <qmap.h>
#include <qvector.h>
#include <qqueue.h>

#include "IEventBusClient.h"

/*! \file */

/*! \class EventBus
\brief Provides a decentralized way to exchange messages between distinct modules of the Mobii application.

The Event bus is some kind of middleware which implements a publish and subscribe architecture, allowing a module to communicate
with another one within the same application without the need to declare an explicit reference to the recipient module, using an
events system. There is no known limit to the number of subscribers for a given event, besides maybe limitations within Qt itself.

Events are unique srings to which arbitrary datas can be associated. For scope limitation reasons, such datas should be pointers
to dynamically allocated variables (in heap). If the method which will receive your event will not use the associated data outside of its scope, these datas may be sent as simple references.
	
This bus relies on the Qt's builtin signals/slots system.

The usage workflow is very simple. First, the subscriber (S) has to implement a method which will process a given event:
	1.	S implements a method with the following prototype:

		\code{.cpp}
		void	doSomething(const QString &eventName, const QObject *me, const char *slot);
		\endcode

		Where:
			-	*eventName* is the name of the unique event.
			-	*me* refers to the object which will send the event.
			-	*slot* is the prototype of the slot which will process the event. This prototype is invariable (see below).

		Here, *me* and *slot* are the "right part" of Qt's *connect* method.
		Then, *me* must be inherit QObject and have the Q_OBJECT macro set.
		Similarly, *slot* must be formatted using the Qt's *SLOT()* macro.

	2.	Then, S registers its dedicated method as a *consumer* of the given event. To do so, S calls the *registerAsConsumer* method:
		
		\code{.cpp}
		this->registerAsConsumer("MY_EVENT_NAME", this, SLOT(doSomething(const QString &, const QObject *, const char *)));
		\endcode

		By calling *registerAsConsumer*, S is now ready to handle any event named "MY_EVENT_NAME".

Now, emitting an event is even more simple, the publisher P only has to call the *emitEvent* method to trigger all subscribers (such as S) which subscribed to that event:

\code{.cpp}
this->m_eventbus->emitEvent("MY_EVENT_NAME", this, myDatas);
\endcode

In this example, *myDatas* can represent a structure, a class instance, an integer, or anything else.

The events bus doesn't communicate the type of the datas which are sent to the subscriber, which has the responsibility to handle the datas appropriately.
 */
class EventBus : public QObject, public IEventBusClient
{
	Q_OBJECT

public:
	EventBus(void);
	virtual ~EventBus(void);

	/// Stores all subscribers prototypes for one single event name.
	typedef QMap<const QObject *, const char *>		TPointersList;

	/// Stores all subscribers prototypes for all events.
	typedef QMap<QString, TPointersList>	TConsumersList;

	/// \brief Registers the sending object as a consumer of `eventName' events.
	virtual void	registerAsConsumer(const QString &eventName, const QObject *me, const char *slot);

	/// Unregisters all connections for this consumer
	virtual void	unregisterConsumer(const QObject *me);

	/// Triggers an event which will be interpreted by receivers.
	/// Used by PUBLISHERS.
	virtual void	emitEvent(const QString &eventName, void* sender, void *data);

	/// Convenient version of the fuller version of this method, with sender and data set to NULL
	/// Used by PUBLISHERS.
	virtual void	emitEvent(const QString &eventName);

protected:
	/// \brief Stores infos about which consumer is interested in X signals.
	///
	/// Key: event name
	///
	/// Value: array of object pointers
	TConsumersList	consumers_;

signals:
	/// \brief Public signal sent by receivers.
	/// Data field can be basically anything. Receiver has the responsibility to free the item.
	/// Used by CONSUMERS.
	void		eventReceived(const QString eventName, void *sender, void *data);
};


#endif // !EVENTBUS_H_