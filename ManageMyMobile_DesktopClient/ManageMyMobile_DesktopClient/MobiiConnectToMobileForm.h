/*
** Mobii Project -- version 0.0.1000 DEVELOPMENT VERSION
**
** Copyright(C) 2012 Mobii contributors.
**
** This library is provided `as is' and without any warranty.
** Authors decline any responsibility for any damage directly or
** indirectly caused by the use of this library.
*/

#ifndef MOBIICONNECTTOMOBILEFORM_H_
#define MOBIICONNECTTOMOBILEFORM_H_

#include		<qwidget.h>
#include		<qmap.h>
#include		<qshortcut.h>

#include		"ui_MobiiConnectToMobileForm.h"
#include		"AModuleDependencyUser.h"

/// Number of columns in table which displays the mobiles list.
#define			UID_COLUMN_IN_HISTORY_ARRAY	4

/*! \file */

/*! \class MobiiConnectToMobileForm
 *  \brief Integrated widget which allows connection to a Mobii-enabeld mobile phone.
 *
 *	This form is made available only when a connection to the Mobii server has been established.
 */
class			MobiiConnectToMobileForm : public QWidget, public AModuleDependencyUser
{
	Q_OBJECT

public:
	MobiiConnectToMobileForm(void);
	virtual ~MobiiConnectToMobileForm(void);

	/// Key: field name, comprised in name, imei, model, manufacturer
	/// Value: contents of the field
	typedef QMap<QString, QString>		TMobileInfos;

	/// Key: UID as string
	/// Value: single mobile informations related to this UID
	typedef QMap<QString, TMobileInfos>	TMobileInfosList;

private:
	Ui::MobiiConnectToMobileForm		ui;

	/// Model which stores the internal representation of the mobiles list
	TMobileInfosList					m_mobilesList;

	/// Delete key detection
	QShortcut *							m_delKeyDetector;

	/// Applies the model stored in m_mobilesList to the GUI table.
	void		updateMobilesListOnUi(void);
	
	/// Returns the currently selected line in the history.
	int			getSelectedRow(void);

	/// Builds a QTableWidgetItem from provided arguments, for a single cell.
	QTableWidgetItem *makeTableCell(const QString &content, bool editable = false);

	/// Saves mobile infos from the internal model to the local drive ONLY
	void		saveMobileInfosOnDisk(void);

	/// Loads mobile infos from the internal model from the local drive ONLY
	void		loadMobileInfosFromDisk(void);

	/// \brief Add a new mobile to the user's devices list.
	/// Sends it to the server if the user is connected to the Mobii network.
	void		saveNewMobile(const TMobileInfos &mobile);

	/// \brief Deletes an existing mobile from the user's devices list.
	/// Notifies the server of the update if the user is connected to the Mobii network.
	void		saveDeletedMobile(const TMobileInfos &mobile);

	/// \brief Renames an existing mobile in the user's devices list.
	/// Notifies the server of the update if the user is connected to the Mobii network.
	void		saveRenameMobile(const TMobileInfos &mobile);

	/// Checks the session controller to determine the login status.
	bool		isUserLoggedIn(void);

private slots:
	void		Slot_mobileInfosReceived(const QString eventName, void *sender, void *data);
	void		Slot_mobileConnected(const QString eventName, void *sender, void *data);
	void		Slot_userLoggedIn(const QString eventName, void *sender, void *data);
	void		Slot_userLoggedOut(const QString eventName, void *sender, void *data);
	void		Slot_parseDevicesListFromJson(const QString eventName, void *sender, void *data);
	void		Slot_ExecuteConnectionForNewPhone(void);

	// Table slots
	void		Slot_cellHasChanged(int row, int col);
	void		Slot_selectionChanged(int row, int col);
	void		Slot_deleteSelectedEntry(void);
	void		Slot_cellDoubleClicked(int row, int col);

protected:
	virtual void OnEventBusSet(void);
	virtual void OnSessionControllerSet(void);
};

#endif // MOBIICONNECTTOMOBILEFORM_H_