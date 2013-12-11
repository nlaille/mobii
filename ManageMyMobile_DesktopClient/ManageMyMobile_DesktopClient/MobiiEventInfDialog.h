#ifndef MOBIIEVENTINFDIALOG_H
#define MOBIIEVENTINFDIALOG_H

#include <qmessagebox.h>
#include <QDialog>
#include "AModuleDependencyUser.h"
#include "ui_MobiiEventInfDialog.h"

class MobiiEventInfDialog : public QDialog, public AModuleDependencyUser
{
	Q_OBJECT

public:
	MobiiEventInfDialog();
	MobiiEventInfDialog(QVariantMap& eventData);
	~MobiiEventInfDialog();

	void		setData(QVariantMap& eventData);
	void		resetData();

private:
	QString						currentID_;
	Ui::MobiiEventInfDialog ui;
private slots:
	void		startTimeChanged(QTime);
	void		endTimeChanged(QTime);
	void		updateCurrentEvent();
	void		deleteCurrentEvent();

protected:
	virtual void closeEvent(QCloseEvent *event);
	/// Specifies actions to perform when the event bus is set.
	virtual void OnEventBusSet(void);
	/// Specifies actions to perform when the session controller is set.
	virtual void OnSessionControllerSet(void);
};

#endif // MOBIIEVENTINFDIALOG_H
