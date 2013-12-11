#include		<stdexcept>
#include		<exception>
#include		<qdebug.h>

#include		"InterclientProtocolHelper.h"
#include		"json.h"


QVariantMap		IcpHelper::jsonToData(QString &inputJson)
{
	bool		success = false;
	QVariantMap	result = QtJson::parse(inputJson, success).toMap();

	if (success == false)
	{
		qDebug() << "Invalid JSON received for parsing:" << inputJson.length() << "bytes:" << inputJson;
		throw std::runtime_error("Invalid JSON received for parsing.");
	}
	return result;
}

QByteArray		IcpHelper::dataToJson(QVariant inputDatas)
{
	QByteArray temp = QtJson::serialize(inputDatas);
	qDebug() << "Translated JSON:" << QString(temp);
	return temp;
}
