/*
** Mobii Project -- version 0.0.1000 DEVELOPMENT VERSION
**
** Copyright(C) 2012 Mobii contributors.
**
** This library is provided `as is' and without any warranty.
** Authors decline any responsibility for any damage directly or
** indirectly caused by the use of this library.
*/

#include <qurl.h>
#include <qdesktopservices.h>

#include "MobiiHomeForm.h"
#include "ConfigurationFileIo.h"

MobiiHomeForm::MobiiHomeForm(QWidget *parent, Qt::WindowFlags flags)
	: QWidget(parent, flags)
{
	this->ui.setupUi(this);
	connect(this->ui.LoginBtn, SIGNAL(clicked()), this, SLOT(User_ProceedLogin()));
	connect(this->ui.CreateNewAccountBtn, SIGNAL(clicked()), this, SLOT(User_RedirectToRegistration()));
	connect(this->ui.UsernameField, SIGNAL(returnPressed()), this, SLOT(User_ProceedLogin()));
	connect(this->ui.PasswordField, SIGNAL(returnPressed()), this, SLOT(User_ProceedLogin()));

	// Load credentials if previously requested
	ConfigurationFileIo cfg;
	this->ui.RememberCredentialsCheckbox->setChecked(cfg.rememberCredentials());
	if (cfg.rememberCredentials() == true)
	{
		this->ui.UsernameField->setText(cfg.savedUsername());
		this->ui.PasswordField->setText(cfg.savedPassword());
	}
}

MobiiHomeForm::~MobiiHomeForm(void)
{
}

void			MobiiHomeForm::User_ProceedLogin(void)
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

void			MobiiHomeForm::User_RedirectToRegistration(void)
{
	QDesktopServices::openUrl(QUrl("http://www.getmobii.com/signup"));
}

void			MobiiHomeForm::OnEventBusSet(void)
{
	
}

void			MobiiHomeForm::OnSessionControllerSet(void)
{
}

