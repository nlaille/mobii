#include <qdebug.h>

#include "MobiiEventBusCodes.h"

QString		MobiiEventBusHelper::eventNameFromAction(const QString &actionCode)
{
	QString generatedEvent(JSON_INPUT_PREFIX);
	generatedEvent += actionCode.toUpper();
	qDebug("JSON event name for action `%s' is `%s'", actionCode.toStdString().c_str(), generatedEvent.toStdString().c_str());
	return generatedEvent.toStdString().c_str();
}
