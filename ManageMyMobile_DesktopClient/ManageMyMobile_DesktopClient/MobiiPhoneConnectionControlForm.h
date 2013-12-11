/*
** Mobii Project -- version 0.0.1000 DEVELOPMENT VERSION
**
** Copyright(C) 2012 Mobii contributors.
**
** This library is provided `as is' and without any warranty.
** Authors decline any responsibility for any damage directly or
** indirectly caused by the use of this library.
*/

#ifndef MOBIIPHONECONNECTIONCONTROLFORM_H_
#define MOBIIPHONECONNECTIONCONTROLFORM_H_

/*! \file */

#include <qwidget.h>
#include <qmap.h>

#include "ui_MobiiPhoneConnectionControl.h"
#include "AModuleDependencyUser.h"

/*! \class MobiiPhoneConnectionControlForm
 *  \brief Displays informations regarding the phone currently connected.
 *
 *	This also provides some controls over the phone connection. It notably
 *	allows disconnection from the connected phone.
 */
class MobiiPhoneConnectionControlForm : public QWidget, public AModuleDependencyUser
{
	Q_OBJECT

public:
	MobiiPhoneConnectionControlForm(void);
	virtual ~MobiiPhoneConnectionControlForm(void);

private:
	/// User interface
	Ui::MobiiPhoneConnectionControlForm ui;

	/// Matrix which contains the common designations for each JSON field
	QMap<QString, QString>	nameMatrix_;

	/// Initializes the header line of the table which contains informations regarding the connected phone.
	void			initializeMatrix(void);

	/// Returns a user-friendly alias for an information field sent by the phone upon connection.
	const QString &	commonNameByKey(const QString &);

private slots:
	/// Handles information datas sent by the phone and displays them on the panel.
	void			do_processMobileInfos(const QString eventName, void *sender, void *data);

	/// Purges the panel, notably the phone informations in the table, when disconnecting from the server and/or the mobile.
	void			OnDisconnected(const QString, void *, void *);

	/// \brief Allows to wipe clean all datas currently saved on the local drive for this phone.
	/// \bug	Even if datas are removed from the drive when clicking on the button, some of them may still be loaded on the UI.
	///			If so, such datas may be saved again on the drive, undoing the wipe clean, as the datas saving process is done when disconnecting the client from the phone.
	void			OnPurgeAllDatasClicked(void);

protected:
	/// Specifies actions to perform when the event bus is set.
	virtual void OnEventBusSet(void);
};

#endif // !MOBIIPHONECONNECTIONCONTROLFORM_H_