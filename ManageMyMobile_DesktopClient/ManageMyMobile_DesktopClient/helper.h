#ifndef HELPER_H
#define HELPER_H

#include <QObject>
#include <qstring.h>
#include <qmessagebox.h>
#include <qdatetime.h>

class Helper
{
public:
	Helper();
	~Helper();

	static QString ToStringISODateWithTimeZone(QDateTime date);
	static void ExecuteInfoMsgBox(QString text);

private:
	
};

#endif // HELPER_H
