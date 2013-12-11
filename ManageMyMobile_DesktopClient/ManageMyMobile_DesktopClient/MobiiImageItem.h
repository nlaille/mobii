#ifndef MOBIIIMAGEITEM_H
#define MOBIIIMAGEITEM_H

#include <QObject>
#include <qdatetime.h>
#include <qstring.h>

class MobiiImageItem
{
public:
	MobiiImageItem();
	MobiiImageItem(int, QString, QDateTime);
	~MobiiImageItem();

	int			id;
	QString		imageName;
	QDateTime	date;

};

#endif // MOBIIIMAGEITEM_H
