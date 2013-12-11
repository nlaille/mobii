#include <qapplication.h>
#include <qplugin.h>

#include "MobiiCore.h"
#include "qlibraryinfo.h"
#include "DebugLogToFile.h"

///
/// \brief Entry point of the Mobii application.
/// Qt components are loaded from this point.
///
int				main(int argc, char *argv[])
{
	// Override qDebug() output
	if (argc >= 2 && ::strcmp(argv[1], "--logfile") == 0)
		qInstallMessageHandler(DebugLogToFile::outputQdebugToFile);

	QApplication app(argc, argv);
	app.setApplicationName("Mobii");

	qDebug() << "Application path is " << QCoreApplication::applicationDirPath();
	qDebug() << "Library path is " << QLibraryInfo::location(QLibraryInfo::PluginsPath);

	MobiiCore core;
	core.setupApplicationHandle(app);
	core.start();
	return app.exec();
}
