#ifndef DEVSTYLESHEETEDITOR_H_
#define DEVSTYLESHEETEDITOR_H_

#include		<qdialog.h>

#include		"ui_DevStylesheetEditorForm.h"
#include		"AModuleDependencyUser.h"

/*! \file */

/// \brief Dialog which allows to dynamically change the Qt stylesheet of the application.
/// \deprecated Has never been used extensively, and making it work throughout the application would require implementing event bus subscriptions when the stylesheet is modified.
class			DevStylesheetEditorForm : public QDialog, public AModuleDependencyUser
{
	Q_OBJECT

public:
	explicit DevStylesheetEditorForm(QWidget *qssTarget);

private:
	QWidget *	m_target;

	Ui::DevStylesheetEditorForm ui;

private slots:
	void		do_applyChanges(void);
	void		do_close(void);
};


#endif // !DEVSTYLESHEETEDITOR_H_