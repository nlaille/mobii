#include		<qdebug.h>

#include		"AModuleDependencyUser.h"


AModuleDependencyUser::AModuleDependencyUser(void)
	: eventbus_(NULL), sessioncon_(NULL)
{
}


AModuleDependencyUser::~AModuleDependencyUser(void)
{
}


void		AModuleDependencyUser::setEventBus(IEventBusClient *obj)
{
	if (obj == NULL)
		qDebug() << Q_FUNC_INFO << "CAUTION: providing `argument' object is likely to cause crashes!";
	this->eventbus_ = obj;
	this->OnEventBusSet();
}


void		AModuleDependencyUser::setSessionController(ISessionController *obj)
{
	if (obj == NULL)
		qDebug() << Q_FUNC_INFO << "CAUTION: providing `argument' object is likely to cause crashes!";
	this->sessioncon_ = obj;
	this->OnSessionControllerSet();
}


void		AModuleDependencyUser::OnEventBusSet(void)
{
}


void		AModuleDependencyUser::OnSessionControllerSet(void)
{
}