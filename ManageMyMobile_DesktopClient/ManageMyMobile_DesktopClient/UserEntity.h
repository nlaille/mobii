#ifndef USERENTITY_H_
#define USERENTITY_H_

#include	<qstring.h>
#include	<qlist.h>
#include	<qdir.h>

#include	"DeviceEntity.h"

/// Default directory name used for devices which are not linked to a Mobii network account
#define		USERENTITY_DEFAULT	"[default_user]"

/*! \file */

///
/// \brief Represents a user which has an account on the Mobii network and provides a set of functionalities to manage the list of devices the user owns.
///
class		UserEntity
{
public:
	~UserEntity(void);

	/// Creates an internal entity for the given user.
	static UserEntity *		loadUser(const QString &username, const QDir &appDir);
	
	/// Loads the default user used when not authenticated.
	/// Shortcut for loadUser(USERENTITY_DEFAULT, appDir).
	static UserEntity *		loadDefaultUser(const QDir &appDir);

	template		<typename StorageType>
	StorageType *	readDatasForModule(const QString &moduleName)
	{
		QFile file(this->userDir_.filePath(moduleName));
		return DataFileSerializer::deserialize<StorageType>(file);
	}

	template		<typename StorageType>
	bool			writeDatasForModule(const QString &moduleName, const StorageType &datas)
	{
		QString filepath = this->userDir_.filePath(moduleName);
		QFile file(filepath);
		return DataFileSerializer::serialize<StorageType>(datas, file);
	}

	/// Refreshes the devices list for this user.
	void		refreshDevicesList(void);

	/// Moves the specified device to this user directory.
	bool		setOwnerForDevice(const DeviceEntity &device);

	/// Creates a new device belonging to this user.
	bool		addDevice(const QByteArray &uid);

	// Getters
	const QString &				username(void) const;
	const QList<DeviceEntity> &	devices(void) const;
	DeviceEntity *				deviceByUid(const QByteArray &uid);

private:
	explicit UserEntity(const QString &username, const QDir &userDir);

	QDir		userDir_;
	QString		username_;
	QList<DeviceEntity>	devices_;
};


#endif // !USERENTITY_H_