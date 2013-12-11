#ifndef MOBIINEWSMSDIALOG_H_
#define MOBIINEWSMSDIALOG_H_

#include <qdialog.h>

#include "ui_MobiiNewSmsDialog.h"
#include "IEventBusClient.h"

/*! \file */

/// International standard size for a single SMS
#define MAX_SMS_LENGTH			160

/// Maximum number of SMS to send withing one packet
#define MAX_SMS_UNITS			5

/// \brief Dialog which allows the user to write a new SMS.
class MobiiNewSmsDialog : public QDialog
{
	Q_OBJECT

public:
	explicit MobiiNewSmsDialog(IEventBusClient *eventbus);
	MobiiNewSmsDialog(IEventBusClient *eventbus, QString phoneNb);
	~MobiiNewSmsDialog(void);

private:
	Ui::NewSmsDialog		ui;
	IEventBusClient *		eventbus_;

private slots:
	void			OnSendSmsClicked();
	void			OnCancelSmsClicked();
	void			OnTextChanged(void);
};

#endif // !MOBIINEWSMSDIALOG_H_