#include		<qdebug.h>
#include		<qwidget.h>

#include		"AExtendedUiWidget.h"


void		AExtendedUiWidget::OnWidgetShow(void)
{
	QWidget *widget = dynamic_cast<QWidget *>(this);
	qDebug() << "UI Widget opened:" << (widget != NULL ? widget->objectName() : "<unknown -- not a QWidget>");
}


void		AExtendedUiWidget::OnWidgetClose(void)
{
	QWidget *widget = dynamic_cast<QWidget *>(this);
	qDebug() << "UI Widget closed:" << (widget != NULL ? widget->objectName() : "<unknown -- not a QWidget>");
}