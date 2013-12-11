#ifndef MOBIICALENDARPANEL_H
#define MOBIICALENDARPANEL_H

#include <qlistwidget.h>
#include <qdatetime.h>
#include <QTextCharFormat>
#include <QWidget>
#include "commons.h"
#include "ui_MobiiCalendarPanel.h"
#include "MobiiEventInfDialog.h"
#include "AModuleDependencyUser.h"
#include "AExtendedUiWidget.h"
#include "MobiiEventBusCodes.h"

class MobiiCalendarPanel : public QWidget, public AModuleDependencyUser, public AExtendedUiWidget
{
	Q_OBJECT

public:
	MobiiCalendarPanel();
	~MobiiCalendarPanel();

private slots:
	void		validateNewEvent(const QString eventName, void *sender, void *data);
	void		CalendarReceived(const QString eventName, void *sender, void *data);
	void		createNewEvent();
	void		updateMobileEvents();
	void		requestMobileEvents();
	void		daySelectedChanged();
	void		eventSelectedChanged();
	void		OnDisconnected(const QString eventName, void *, void *);
	void		OnConnected(const QString eventName, void *sender, void *data);
	void		updateCurrentEvent(const QString eventName, void *sender, void *data);
	void		deleteCurrentEvent(const QString eventName, void *sender, void *data);

private:
	// Used for filesystem backup
	typedef QVariant					TCalendarBackupItem;
	typedef QList<TCalendarBackupItem>	TCalendarBackupContainer;
	TCalendarBackupContainer			rawCalendarDatas_;

	bool					firstTimeInWidget;
	int						currentEventId_;
	MobiiEventInfDialog		*eventPanel_;
	QList<QVariant>			*src_;
	Ui::MobiiCalendarPanel ui;

protected:
	virtual void	OnWidgetShow(void);
	virtual void	OnWidgetClose(void);
	/// Specifies actions to perform when the event bus is set.
	virtual void OnEventBusSet(void);
	
	/// Specifies actions to perform when the session controller is set.
	virtual void OnSessionControllerSet(void);
};

#endif // MOBIICALENDARPANEL_H
