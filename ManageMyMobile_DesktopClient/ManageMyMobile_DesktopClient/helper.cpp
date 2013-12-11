#include "helper.h"

Helper::Helper()
{
}

Helper::~Helper()
{
}

QString Helper::ToStringISODateWithTimeZone(QDateTime date)
{
	QDateTime utc = date.toUTC();
	utc.setTimeSpec(Qt::LocalTime);
	int utcOffset = utc.secsTo(date);

	date.setUtcOffset(utcOffset);
	return (date.toString(Qt::ISODate));
}

void Helper::ExecuteInfoMsgBox(QString text)
{
	QMessageBox tmp;
	tmp.setText(text);
	tmp.exec();
}
