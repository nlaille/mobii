#include <stdexcept>
#include <exception>
#include <qobject.h>
#include <qdebug.h>

#include "EventBus.h"


EventBus::EventBus(void)
	: QObject()
{
}


EventBus::~EventBus(void)
{
	QObject::disconnect(SIGNAL(eventReceived(const QString, void *, void *)));
}


void		EventBus::registerAsConsumer(const QString &eventName, const QObject *me, const char *slot)
{
	if (this->consumers_.contains(eventName)
		&& this->consumers_[eventName].contains(me))
		qDebug() << "Event Bus: event `" << eventName << "'is already being listened by this consumer.";
	else
		this->consumers_[eventName][me] = slot;
}


void		EventBus::unregisterConsumer(const QObject *me)
{
	for (TConsumersList::iterator it = this->consumers_.begin(); it != this->consumers_.end(); ++it)
	{
		TPointersList plist = it.value();
		bool hasRemovedItems = false;
		do
		{
			for (TPointersList::iterator pli = plist.begin(); pli != plist.end(); ++pli)
			{
				if (pli.key() == me)
				{
					plist.erase(pli);
					hasRemovedItems = true;
					break;
				}
			}
			hasRemovedItems = false;
		}
		while (hasRemovedItems == true);
	}
}


void        EventBus::emitEvent(const QString &eventName, void* sender, void *data)
{
    for (TConsumersList::iterator it = this->consumers_.begin(); it != this->consumers_.end(); ++it)
    {
        if (it.key() == eventName)
        {
            TPointersList plist = it.value();
            QObject::disconnect(SIGNAL(eventReceived(const QString, void *, void *)));
            for (TPointersList::iterator it2 = plist.begin(); it2 != plist.end(); ++it2)
            {
                QObject::connect(
                    this, SIGNAL(eventReceived(const QString, void *, void *)),
                    it2.key(), it2.value());
				emit eventReceived(eventName, sender, data);
				QObject::disconnect(SIGNAL(eventReceived(const QString, void *, void *)));
            }
			qDebug() << "Emitted signal `" << eventName << "' to " << it.value().count() << "consumers.";
        }
    }
}


void		EventBus::emitEvent(const QString &eventName)
{
	this->emitEvent(eventName, NULL, NULL);
}