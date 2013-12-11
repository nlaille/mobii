#ifndef DEBUGLOGTOFILE_H_
#define DEBUGLOGTOFILE_H_

#include <qfile.h>
#include <qtextstream.h>
#include <qdatetime.h>

/*! \file */

///
/// \brief Allows logs to be printed in a separate file, instead of stderr.
/// You do not need to setup this module by yourself unless you want to modify its behavior.
/// This module is automatically initialized in the main function.
///
namespace DebugLogToFile
{
	extern QFile *	fHandle;

	void			outputQdebugToFile(QtMsgType type, const QMessageLogContext &context, const QString &msg);
}


#endif // !DEBUGLOGTOFILE_H_