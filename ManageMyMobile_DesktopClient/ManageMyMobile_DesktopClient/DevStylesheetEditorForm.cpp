#include		"DevStylesheetEditorForm.h"


DevStylesheetEditorForm::DevStylesheetEditorForm(QWidget *qssTarget)
	: m_target(qssTarget)
{
	this->ui.setupUi(this);
	this->ui.stylesheetEdit->setPlainText(this->m_target->styleSheet());
	connect(this->ui.applyBtn, SIGNAL(clicked()), this, SLOT(do_applyChanges()));
	connect(this->ui.applyCloseBtn, SIGNAL(clicked()), this, SLOT(do_close()));
}


void		DevStylesheetEditorForm::do_applyChanges(void)
{
	this->m_target->setStyleSheet(this->ui.stylesheetEdit->toPlainText());
}


void		DevStylesheetEditorForm::do_close(void)
{
	this->do_applyChanges();
	this->close();
}