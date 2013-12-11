#include "MobiiImageItem.h"

MobiiImageItem::MobiiImageItem()
{

}

MobiiImageItem::MobiiImageItem(int i, QString str, QDateTime qdt)
{
	this->id = i;
	this->imageName = str;
	this->date = qdt;
}

MobiiImageItem::~MobiiImageItem()
{

}
