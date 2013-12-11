/*
** Mobii Project -- version 0.0.1000 DEVELOPMENT VERSION
**
** Copyright(C) 2012 Mobii contributors.
**
** This library is provided `as is' and without any warranty.
** Authors decline any responsibility for any damage directly or
** indirectly caused by the use of this library.
*/

#ifndef MOBII_HOME_FORM
#define MOBII_HOME_FORM

#include <qwidget.h>

#include "MobiiEventBusCodes.h"
#include "ui_MobiiHomeForm.h"
#include "AModuleDependencyUser.h"

/*! \file */

/*! \class MobiiHomeForm
 *  \brief Home page widget
 *
 *  Handles behavior of the home screen displayed to the user upon startup.
 */
class MobiiHomeForm : public QWidget, public AModuleDependencyUser
{
	Q_OBJECT

public:
	/*!
     *  \brief Constructor
     *
     *  Constructor for MobiiHomeForm class
     *
     *  \param parent : parent QWidget - see official QT documentation
	 *  \param flags : QT window flags - see official QT documentation
     */
	MobiiHomeForm(QWidget *parent = 0, Qt::WindowFlags flags = 0);

	/*!
     *  \brief Destructor
     *
     *  Destructor for MobiiHomeForm class
     */
	virtual ~MobiiHomeForm(void);

private:
	/// QT user interface object
	Ui::MobiiHomeForm ui;

private slots:
	/// Sends a login request to the server using user-specified credentials.
	void		User_ProceedLogin(void);

	/// Redirects the user to the Mobii network registration page.
	void		User_RedirectToRegistration(void);

protected:
	/// Specifies actions to perform when the event bus is set.
	virtual void OnEventBusSet(void);

	/// Specifies actions to perform when the session controller is set.
	virtual void OnSessionControllerSet(void);
};

#endif // MOBII_HOME_FORM
