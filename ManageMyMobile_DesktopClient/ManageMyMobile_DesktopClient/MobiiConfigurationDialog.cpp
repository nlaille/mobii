/*
** Mobii Project -- version 0.0.1000 DEVELOPMENT VERSION
**
** Copyright(C) 2012 Mobii contributors.
**
** This library is provided `as is' and without any warranty.
** Authors decline any responsibility for any damage directly or
** indirectly caused by the use of this library.
*/

#include <qmessagebox.h>
#include <qfiledialog.h>
#include <qradiobutton.h>
#include <qlist.h>

#include "commons.h"
#include "MobiiConfigurationDialog.h"
#include "MobiiEventBusCodes.h"


MobiiConfigurationDialog::MobiiConfigurationDialog(void)
{
	this->ui.setupUi(this);

	connect(this->ui.UseDefaultServerSettingsRadio, SIGNAL(clicked()), this, SLOT(Slot_SetServerSettingsVisibilityHidden()));
	connect(this->ui.UseCustomServerSettingsRadio, SIGNAL(clicked()), this, SLOT(Slot_SetServerSettingsVisibilityShown()));
	connect(this->ui.okButton, SIGNAL(clicked()), this, SLOT(Slot_ValidateChangesThenClose()));
	connect(this->ui.selectStorageDirectoryBtn, SIGNAL(clicked()), this, SLOT(Slot_ChooseUserStorageDirectory()));
	connect(this->ui.setUserStorageDirToDefaultBtn, SIGNAL(clicked()), this, SLOT(Slot_SetDefaultUserStorageDir()));
}

MobiiConfigurationDialog::~MobiiConfigurationDialog(void)
{
}

void		MobiiConfigurationDialog::OnWidgetShow(void)
{
	this->loadSettings();
}

void		MobiiConfigurationDialog::loadSettings()
{
	// General
	this->ui.uidField->setText(Commons::hexToLitteralString(this->m_cfg.clientUid().data(), this->m_cfg.clientUid().size(), " ").c_str());
	this->ui.uidField->setText(this->ui.uidField->text().toUpper());
	this->ui.userStorageDirField->setText(this->m_cfg.userStorageDir());
	this->ui.savePhonePasswordsOpt->setChecked(this->m_cfg.phonePasswordsAutosave());

	if (this->m_cfg.serverModeCustom() == false)
	{
		this->ui.UseDefaultServerSettingsRadio->setChecked(true);
		this->ui.CustomServerSettingsGroup->setEnabled(false);
	}
	else
	{
		this->ui.UseCustomServerSettingsRadio->setChecked(true);
		this->ui.CustomServerSettingsGroup->setEnabled(true);
		this->ui.ServerAddressField->setText(this->m_cfg.serverAddress());
		this->ui.ServerPortField->setText(QString("%1").arg(this->m_cfg.serverTcpPort()));
		this->ui.DirectConnectionPortField->setText(QString("%1").arg(this->m_cfg.directConnectionPort()));
		this->ui.UseSslCheckBox->setChecked(this->m_cfg.useSsl());
	}

	// Panels
	quint16 nbToLoad = this->m_cfg.smsPanelNbOfMessagesToDisplay();
	QRadioButton * foundSmsNbRadios = this->ui.nbSmsToDisplayArea->findChild<QRadioButton *>(QString("radioButton") + QString::number(nbToLoad));
	if (foundSmsNbRadios != NULL)
		foundSmsNbRadios->setChecked(true);
}

void		MobiiConfigurationDialog::saveSettings()
{
	// General
	this->m_cfg.setPhonePasswordsAutosave(this->ui.savePhonePasswordsOpt->isChecked());
	this->m_cfg.setUserStorageDir(this->ui.userStorageDirField->text());

	if (this->ui.UseDefaultServerSettingsRadio->isChecked())
	{
		this->m_cfg.setServerModeCustom(false);
		this->eventbus_->emitEvent(CORE_REQ_RESETSRVSETTINGS, this, NULL);
	}
	else
	{
		this->m_cfg.setServerModeCustom(true);
		this->m_cfg.setServerAddress(this->ui.ServerAddressField->text());
		this->m_cfg.setServerTcpPort(this->ui.ServerPortField->text().toUShort());
		this->m_cfg.setDirectConnectionPort(this->ui.DirectConnectionPortField->text().toUShort());
		this->m_cfg.setUseSsl((this->ui.UseSslCheckBox->checkState()) == Qt::Unchecked ? false : true);
	}
	
	QList<QRadioButton *> allSmsNbRadios = this->ui.nbSmsToDisplayArea->findChildren<QRadioButton *>(QRegExp("radioButton.*"));
	for (QList<QRadioButton *>::iterator it = allSmsNbRadios.begin(); it != allSmsNbRadios.end(); ++it)
	{
		if ((*it)->isChecked())
		{
			this->m_cfg.setSmsPanelNbOfMessagesToDisplay((*it)->text().toUInt());
			break;
		}
	}
}

void		MobiiConfigurationDialog::Slot_SetServerSettingsVisibilityHidden(void)
{
	this->ui.CustomServerSettingsGroup->setEnabled(false);
}

void		MobiiConfigurationDialog::Slot_SetServerSettingsVisibilityShown(void)
{
	this->ui.CustomServerSettingsGroup->setEnabled(true);
}

void		MobiiConfigurationDialog::Slot_ChooseUserStorageDirectory(void)
{
	QFileDialog dialog;
	dialog.setFileMode(QFileDialog::Directory);
	dialog.setOption(QFileDialog::ShowDirsOnly);
	if (dialog.exec())
		this->ui.userStorageDirField->setText(dialog.directory().absolutePath());
}

void		MobiiConfigurationDialog::Slot_SetDefaultUserStorageDir(void)
{
	// Requests reinitialization of the user storage directory
	this->eventbus_->emitEvent(CORE_REQ_RESETUSERSTOR, this, NULL);

	// Updates the UI to reflect the new path
	this->ui.userStorageDirField->setText(this->m_cfg.userStorageDir());
}

void		MobiiConfigurationDialog::Slot_ValidateChangesThenClose(void)
{
	// Custom server settings
	if (this->ui.UseCustomServerSettingsRadio->isChecked() == true)
	{
		QRegExp regexp("\\b(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\b");
		if (!(this->ui.ServerAddressField->text().contains(regexp)))
		{
			QMessageBox msg("Champ invalide", "Le champ d'adresse IP doit être composé de 4 champs allant de 0 à 255, séparés par des points.", QMessageBox::Warning, QMessageBox::Ok, 0, 0);
			msg.exec();
			return;
		}
		int portValue = this->ui.ServerPortField->text().toInt();
		int directPortValue = this->ui.DirectConnectionPortField->text().toInt();
		if (portValue < 1 || directPortValue < 1 || portValue > (quint16)-1 || directPortValue > (quint16)-1)
		{
			QMessageBox msg("Champ invalide", "Les numéros de port doivent être des nombres entre 1 et 65535.", QMessageBox::Warning, QMessageBox::Ok, 0, 0);
			msg.exec();
			return;
		}
	}

	// Save then exit
	this->saveSettings();
	this->close();
}