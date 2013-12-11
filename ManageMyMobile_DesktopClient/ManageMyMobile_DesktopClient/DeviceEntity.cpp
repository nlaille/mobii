#include <qdebug.h>

#include "commons.h"
#include "DeviceEntity.h"


DeviceEntity::DeviceEntity(const QByteArray &uid, const QDir &deviceDir)
	: deviceId_(uid), deviceDir_(deviceDir)
{
}


DeviceEntity::DeviceEntity(const DeviceEntity &cpy)
{
	if (&cpy != this)
	{
		this->deviceDir_ = cpy.locationOnFilesystem();
		this->deviceId_ = cpy.id();
	}
}


DeviceEntity::~DeviceEntity(void)
{
}


DeviceEntity *	DeviceEntity::loadDevice(const QByteArray &uid, const QDir &userDir)
{
	// Determine final device location
	QDir deviceDir(userDir);
	if (deviceDir.cd(DeviceEntity::directoryNameForUid(uid)) == false)
	{
		// Create the directory, report failure if unable
		if (deviceDir.mkpath(DeviceEntity::directoryNameForUid(uid)) == false)
		{
			qDebug() << "Unable to create device directory:" << deviceDir.absolutePath();
			return NULL;
		}
		deviceDir.cd(DeviceEntity::directoryNameForUid(uid));
	}
	DeviceEntity *entity = new DeviceEntity(uid, deviceDir);
	return entity;
}


DeviceEntity *	DeviceEntity::loadDevice(const QString &uidAsString, const QDir &userDir)
{
	std::string s(Commons::stringToLitteralHex(uidAsString.toStdString()));
	QByteArray arr = QByteArray(s.data(), s.length());
	return DeviceEntity::loadDevice(arr, userDir);
}


QString			DeviceEntity::directoryNameForUid(const QByteArray &uid)
{
	return QString(Commons::hexToLitteralString(uid.data(), uid.length(), "").c_str()).toUpper();
}


QByteArray		DeviceEntity::byteArrayFromString(const QString &s)
{
	std::string tmp = Commons::stringToLitteralHex(s.toStdString());
	return QByteArray(tmp.c_str(), tmp.size());
}


bool			DeviceEntity::purgeLocalDatas(void)
{
	bool res;
	res = this->deviceDir_.removeRecursively();
	res &= this->deviceDir_.mkpath(this->deviceDir_.absolutePath());
	return res;
}


QString			DeviceEntity::locationOnFilesystem(void) const
{
	return this->deviceDir_.absolutePath();
}


const QByteArray &DeviceEntity::id(void) const
{
	return this->deviceId_;
}

bool		operator == (DeviceEntity &d1, DeviceEntity &d2)
{
	return d1.id() == d2.id();
}