#ifndef DATAFILESERIALIZER_H_
#define DATAFILESERIALIZER_H_

#include <qdatastream.h>
#include <qdir.h>
#include <qdebug.h>

/*! \file */

/// \brief Provides functionality to serialize panel datas to files and read from them.
namespace DataFileSerializer
{
	/// Serializes the specified object to the requested destination file.
	template	<typename FinalType>
	bool		serialize(const FinalType &item, QFile &file)
	{
		// Prepare file
		if (file.open(QIODevice::WriteOnly) == false)
		{
			qDebug() << Q_FUNC_INFO << "Unable to open file `" << file.fileName() << "'";
			return false;
		}

		// Serialize datas to file
		QDataStream stream(&file);
		stream.setByteOrder(QDataStream::LittleEndian);
		stream.setVersion(QDataStream::Qt_5_0);
		stream << item;
		file.close();
		return true;
	}

	/// Deserializes the contents of the given file.
	template	<typename FinalType>
	FinalType *	deserialize(QFile &file)
	{
		// Prepare file
		if (file.open(QIODevice::ReadOnly) == false)
		{
			qDebug() << Q_FUNC_INFO << "Unable to open file `" << file.fileName() << "'";
			return NULL;
		}

		// Deserialize from file
		QDataStream stream(&file);
		stream.setByteOrder(QDataStream::LittleEndian);
		stream.setVersion(QDataStream::Qt_5_0);
		FinalType *obj = new FinalType();
		stream >> (*obj);
		return obj;
	}
}


#endif // !DATAFILESERIALIZER_H_