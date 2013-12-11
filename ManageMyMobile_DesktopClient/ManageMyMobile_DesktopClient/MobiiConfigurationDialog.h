/*
** Mobii Project -- version 0.0.1000 DEVELOPMENT VERSION
**
** Copyright(C) 2012 Mobii contributors.
**
** This library is provided `as is' and without any warranty.
** Authors decline any responsibility for any damage directly or
** indirectly caused by the use of this library.
*/

/*! \file */

#ifndef MOBIICONFIGURATIONDIALOG_H_
#define MOBIICONFIGURATIONDIALOG_H_

#include <qdialog.h>

#include "ui_MobiiConfigurationDialog.h"
#include "ConfigurationFileIo.h"
#include "AModuleDependencyUser.h"
#include "AExtendedUiWidget.h"

/*! \class MobiiConfigurationDialog
 *  \brief Provides a user interface allowing various settings changes.
 */
class MobiiConfigurationDialog : public QDialog, public AExtendedUiWidget, public AModuleDependencyUser
{
	Q_OBJECT

public:
	/// QT initialization
	MobiiConfigurationDialog(void);
	virtual ~MobiiConfigurationDialog(void);

	virtual void OnWidgetShow(void);

private slots:
	/// \brief Disables edition for the custom server settings.
	/// Happens when the Default settings radio button is selected.
	void Slot_SetServerSettingsVisibilityHidden(void);

	/// \brief Enables edition for the custom server settings.
	/// Happens when the Custom server settings radio button is selected.
	void Slot_SetServerSettingsVisibilityShown(void);

	/// Allows user to choose a user datas storage directory.
	void Slot_ChooseUserStorageDirectory(void);

	/// Persists changes to the configuration file then closes the dialog.
	void Slot_ValidateChangesThenClose(void);

	/// Resets the user storage directory to defaults.
	void Slot_SetDefaultUserStorageDir(void);

private:
	/// QT user interface object
	Ui::ConfigurationDialog		ui;

	/// Configuration file on which settings are read from and persisted to.
	ConfigurationFileIo			m_cfg;

	/// Loads settings from the configuration file.
	void loadSettings();

	/// Saves the modified settings to the configuration file.
	void saveSettings();
};

#endif // MOBIICONFIGURATIONDIALOG_H_