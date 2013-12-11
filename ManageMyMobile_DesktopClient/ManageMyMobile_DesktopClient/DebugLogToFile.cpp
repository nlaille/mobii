#include <qtextstream.h>

#include "DebugLogToFile.h"


QFile *	DebugLogToFile::fHandle = NULL;

void			DebugLogToFile::outputQdebugToFile(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
	QByteArray localMsg = msg.toLocal8Bit();
	if (DebugLogToFile::fHandle == NULL)
	{
		DebugLogToFile::fHandle = new QFile(".//client.log");
		if (!DebugLogToFile::fHandle->open(QIODevice::WriteOnly | QIODevice::Text))
			abort();
		QTextStream stream(DebugLogToFile::fHandle);
		stream << "*****************************************************\n";
		stream << "Mobii instance launched at " << QDateTime::currentDateTime().toString() << "\n";
	}
	QTextStream s2(DebugLogToFile::fHandle);
	s2 << localMsg.constData() << "\n";
}