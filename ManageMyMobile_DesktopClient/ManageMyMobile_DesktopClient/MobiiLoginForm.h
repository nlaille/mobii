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

#ifndef MOBIILOGINFORM_H_
#define MOBIILOGINFORM_H_

#include <qdialog.h>

#include "ui_MobiiLoginDialog.h"
#include "AModuleDependencyUser.h"

/*! \class MobiiLoginForm
 *  \brief Login credentials dialog window
 *
 *  This class offers the user the ability to log into the Mobii server.
 *	\deprecated Login functionality is now transferred to the Home panel. This class should be removed in future revisions.
 */
class MobiiLoginForm : public QDialog, public AModuleDependencyUser
{
	Q_OBJECT

public:
	/*!
     *  \brief Constructor
     *
     *  Constructor for MobiiLoginForm class
     *
     *  \param parent : parent QWidget - see official QT documentation
	 *  \param flags : QT window flags - see official QT documentation
     */
	MobiiLoginForm(void);

	/*!
     *  \brief Destructor
     *
     *  Destructor for MobiiLoginForm class
     */
	virtual ~MobiiLoginForm(void);

public slots:
	/*!
	 * \brief Proceeds to login attempt
	 *
	 * Begins the authentication process with the given credentials.
	 * Currently, credentials are ignored as there is no strong authentication process.
	 */
	void		User_ProceedLogin(void);

	/*!
     *  \brief Cancels login process
     *
     *  Closes the Login form without attempting to connect to the server.
     */
	void		User_AbortOperation(void);

private:
	/// QT user interface object
	Ui::LoginDialog		ui;

	/// Triggered when the form is shown
	virtual void		showEvent(QShowEvent *);
};

#endif // MOBIILOGINFORM_H_
