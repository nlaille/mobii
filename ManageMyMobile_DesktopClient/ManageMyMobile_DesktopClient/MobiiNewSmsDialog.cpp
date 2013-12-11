#include <qmessagebox.h>

#include "commons.h"
#include "MobiiEventBusCodes.h"
#include "MobiiNewSmsDialog.h"


MobiiNewSmsDialog::MobiiNewSmsDialog(IEventBusClient *eventbus)
{
	this->ui.setupUi(this);
	this->eventbus_ = eventbus;
	this->setStyleSheet(UI_DEFAULT_STYLE);
	connect(this->ui.sendSmsBtn, SIGNAL(clicked()), this, SLOT(OnSendSmsClicked()));
	connect(this->ui.cancelSmsButton, SIGNAL(clicked()), this, SLOT(OnCancelSmsClicked()));
	connect(this->ui.smsContentField, SIGNAL(textChanged()), this, SLOT(OnTextChanged()));

	this->OnTextChanged();
}

MobiiNewSmsDialog::MobiiNewSmsDialog(IEventBusClient *eventbus, QString phoneNb)
{
	this->ui.setupUi(this);
	this->eventbus_ = eventbus;
	this->setStyleSheet(UI_DEFAULT_STYLE);
	connect(this->ui.sendSmsBtn, SIGNAL(clicked()), this, SLOT(OnSendSmsClicked()));
	connect(this->ui.cancelSmsButton, SIGNAL(clicked()), this, SLOT(OnCancelSmsClicked()));
	connect(this->ui.smsContentField, SIGNAL(textChanged()), this, SLOT(OnTextChanged()));

	this->ui.destinationField->setText(phoneNb);
}


MobiiNewSmsDialog::~MobiiNewSmsDialog(void)
{
}


void		MobiiNewSmsDialog::OnSendSmsClicked(void)
{
	if (this->ui.destinationField->text().isEmpty()
		|| this->ui.smsContentField->toPlainText().isEmpty())
	{
		QMessageBox mbox(QMessageBox::Warning,
			"Champ vide",
			"Les champs destinataire et contenu du message doivent être remplis.");
		mbox.exec();
		return;
	}
	if (this->ui.smsContentField->toPlainText().length() > MAX_SMS_LENGTH * MAX_SMS_UNITS)
	{
		QMessageBox mbox(QMessageBox::Warning,
			"Contenu du SMS trop long",
			QString("Le contenu d'un SMS ne peut excéder ").append(MAX_SMS_LENGTH * MAX_SMS_UNITS).append(" caractères."));
		mbox.exec();
		return;
	}
	
	// Send JSON request
	QVariantMap *data = new QVariantMap();
	(*data)["action"] = "req_sendsms";
	(*data)["dest"] = this->ui.destinationField->text();
	(*data)["body"] = this->ui.smsContentField->toPlainText();
	this->eventbus_->emitEvent(JSON_SEND_REQUEST, this, data);

	// Notify success
	QMessageBox mbox(QMessageBox::Information,
		"Message envoyé",
		"Votre message a bien été envoyé par votre téléphone.");
	mbox.exec();
	this->close();
}


void		MobiiNewSmsDialog::OnCancelSmsClicked(void)
{
	QMessageBox mbox(
		"Confirmation de l'annulation",
		"Le contenu de votre SMS sera perdu. Souhaitez-vous vraiment annuler ?",
		QMessageBox::Warning, 
		QMessageBox::Yes, QMessageBox::No, QMessageBox::NoButton);
	mbox.exec();

	// Get pressed button and perform appropriate action
	QAbstractButton *clickedBtn = mbox.clickedButton();
	if (mbox.standardButton(clickedBtn) == QMessageBox::Yes)
		this->close();
}


void		MobiiNewSmsDialog::OnTextChanged(void)
{
	quint32 length = this->ui.smsContentField->toPlainText().length();
	if (length > MAX_SMS_LENGTH * MAX_SMS_UNITS)
		this->ui.smsContentField->textCursor().deletePreviousChar();
	this->ui.sizeLimitLbl->setText(QString::number(MAX_SMS_LENGTH * MAX_SMS_UNITS - this->ui.smsContentField->toPlainText().length()) + QString(" caractères restants."));
	this->ui.nbOfSentSmsLbl->setText(QString("Ce message sera envoyé sur ") + QString::number(length / MAX_SMS_LENGTH + (length % MAX_SMS_LENGTH == 0 ? 0 : 1)) + " SMS.");
}
