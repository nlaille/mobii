#ifndef MOBIIEVENTITEM_H
#define MOBIIEVENTITEM_H

#include <qdatetime.h>
#include <qstring.h>

class MobiiEventItem
{
public:
	MobiiEventItem();
	~MobiiEventItem();

	QDateTime	startdate;
	QDateTime	enddate;
	QString		name;

private:
	
};

#endif // MOBIIEVENTITEM_H
