#include <stdexcept>
#include <exception>

#include "UserEntity.h"


UserEntity::UserEntity(const QString &username, const QDir &userDir)
	: username_(username), userDir_(userDir)
{
}


UserEntity::~UserEntity(void)
{
}


UserEntity *UserEntity::loadUser(const QString &username, const QDir &appDir)
{
	// Determine final user location
	QDir userDir(appDir);
	if (userDir.cd(username) == false)
	{
		// Create the user directory, report failure if unable
		if (userDir.mkpath(username) == false)
		{
			qDebug() << "Unable to create device directory:" << userDir.absolutePath();
			return NULL;
		}
		userDir.cd(username);
	}

	// Create entity
	UserEntity *user = new UserEntity(username, userDir);
	user->refreshDevicesList();
	return user;
}


UserEntity *UserEntity::loadDefaultUser(const QDir &appDir)
{
	return UserEntity::loadUser(USERENTITY_DEFAULT, appDir);
}


void		UserEntity::refreshDevicesList(void)
{
	this->devices_.clear();
	this->userDir_.setFilter(QDir::Dirs);
	QFileInfoList list = this->userDir_.entryInfoList();
	for (int i = 0; i < list.size(); ++i)
	{
		QFileInfo file = list.at(i);
		DeviceEntity *device = DeviceEntity::loadDevice(file.fileName(), this->userDir_);
		if (device == NULL)
		{
			qDebug() << Q_FUNC_INFO << "Unable to create device entity for `" << file.fileName() << "'";
			continue;
		}
		this->devices_.push_back(*device);
		delete device;
	}
}


bool		UserEntity::setOwnerForDevice(const DeviceEntity &device)
{
	QDir dir;
	if (dir.rename(device.locationOnFilesystem(), this->userDir_.absolutePath()) == false)
		return false;
	return this->addDevice(device.id());
}


bool		UserEntity::addDevice(const QByteArray &uid)
{
	DeviceEntity *moved = DeviceEntity::loadDevice(uid, this->userDir_);
	if (moved == NULL)
		return false;
	this->devices_.append(*moved);
	delete moved;
	return true;
}


const QString &UserEntity::username(void) const
{
	return this->username_;
}


const QList<DeviceEntity> &UserEntity::devices(void) const
{
	return this->devices_;
}


DeviceEntity *UserEntity::deviceByUid(const QByteArray &uid)
{
	for (QList<DeviceEntity>::iterator it = this->devices_.begin(); it != this->devices_.end(); ++it)
	{
		if ((*it).id() == uid)
			return &(*it);
	}
	if (this->addDevice(uid) == false)
	{
		qDebug() << Q_FUNC_INFO << "Unable to create a new device.";
		return NULL;
	}
	for (QList<DeviceEntity>::iterator it = this->devices_.begin(); it != this->devices_.end(); ++it)
	{
		if ((*it).id() == uid)
			return &(*it);
	}
	// This should never happen, but it pleases the compiler.
	return DeviceEntity::loadDevice(uid, this->userDir_);
}