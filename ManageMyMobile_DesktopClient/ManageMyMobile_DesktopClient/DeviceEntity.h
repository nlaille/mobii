#ifndef DEVICEENTITY_H_
#define DEVICEENTITY_H_

#include <qstring.h>
#include <qbytearray.h>
#include <qdir.h>

#include "DataFileSerializer.hpp"

/*! \file */

/// \brief Represents a single phone and provides a set of functionalities to manage datas exchanged with that phone.
class DeviceEntity
{
public:
	DeviceEntity(const DeviceEntity &);
	~DeviceEntity(void);

	/// Creates a DeviceEntity which matches the requested UID, creates the arborescence if needed.
	static DeviceEntity *	loadDevice(const QByteArray &uid, const QDir &userDir);

	/// Derived version of loadDevice() which allows setting the UID as a plain string.
	static DeviceEntity *	loadDevice(const QString &uidAsString, const QDir &userDir);

	/// Reads serialized datas from a file which matches the module name.
	template		<typename StorageType>
	StorageType *	readDatasForModule(const QString &moduleName)
	{
		QFile file(this->deviceDir_.filePath(moduleName));
		return DataFileSerializer::deserialize<StorageType>(file);
	}

	/// Serializes datas and writes them in a file according to the given module name.
	template		<typename StorageType>
	bool			writeDatasForModule(const QString &moduleName, const StorageType &datas)
	{
		QString filepath = this->deviceDir_.filePath(moduleName);
		QFile file(filepath);
		return DataFileSerializer::serialize<StorageType>(datas, file);
	}

	/// Purges all locally stored datas from previous synchronizations.
	bool			purgeLocalDatas(void);

	/// Returns the device storage directory absolute path.
	QString			locationOnFilesystem(void) const;

	/// Returns the device UUID
	const QByteArray &id(void) const;

private:
	explicit DeviceEntity(const QByteArray &uid, const QDir &deviceDir);

	/// Directory where data files for this device are stored.
	QDir			deviceDir_;

	/// Device UID
	QByteArray		deviceId_;

	static QString	directoryNameForUid(const QByteArray &uid);
	static QByteArray byteArrayFromString(const QString &s);
};

// Allows comparison with another device
bool		operator == (DeviceEntity &d1, DeviceEntity &d2);


#endif // !DEVICEENTITY_H_