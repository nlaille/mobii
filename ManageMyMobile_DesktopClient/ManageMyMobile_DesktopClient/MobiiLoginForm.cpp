/*
** Mobii Project -- version 0.0.1000 DEVELOPMENT VERSION
**
** Copyright(C) 2012 Mobii contributors.
**
** This library is provided `as is' and without any warranty.
** Authors decline any responsibility for any damage directly or
** indirectly caused by the use of this library.
*/
#include "commons.h"
#include "MobiiEventBusCodes.h"
#include "MobiiLoginForm.h"
#include "ConfigurationFileIo.h"

MobiiLoginForm::MobiiLoginForm(void)
{
	ui.setupUi(this);
	this->setStyleSheet(UI_DEFAULT_STYLE);

	// Both login and abort buttons close the window
	connect(this->ui.LoginBtn, SIGNAL(clicked()), this, SLOT(User_ProceedLogin()));
	connect(this->ui.LoginBtn, SIGNAL(clicked()), this, SLOT(User_AbortOperation()));
	connect(this->ui.AbortBtn, SIGNAL(clicked()), this, SLOT(User_AbortOperation()));
}

MobiiLoginForm::~MobiiLoginForm(void)
{
}

void		MobiiLoginForm::User_ProceedLogin(void)
{
	// Save credentials if requested
	ConfigurationFileIo cfg;
	cfg.setRememberCredentials(this->ui.RememberCredentialsCheckbox->checkState() == Qt::Checked ? true : false);
	if (this->ui.RememberCredentialsCheckbox->checkState() == Qt::Checked)
	{
		cfg.setSavedUsername(this->ui.UsernameField->text());
		cfg.setSavedPassword(this->ui.PasswordField->text());
	}

	// Process login event
	QVector<QString> *args = new QVector<QString>();
	args->push_back(this->ui.UsernameField->text());
	args->push_back(this->ui.PasswordField->text());
	this->eventbus_->emitEvent(BLOCKING_OPERATION_BEGIN, this, NULL);
	this->eventbus_->emitEvent(CONNECT_TO_SERVER, this, args);
}

void		MobiiLoginForm::User_AbortOperation(void)
{
	this->close();
}

void		MobiiLoginForm::showEvent(QShowEvent *)
{
	ConfigurationFileIo cfg;

	// Load credentials if previously requested
	this->ui.RememberCredentialsCheckbox->setChecked(cfg.rememberCredentials());
	if (cfg.rememberCredentials() == true)
	{
		this->ui.UsernameField->setText(cfg.savedUsername());
		this->ui.PasswordField->setText(cfg.savedPassword());
	}
}