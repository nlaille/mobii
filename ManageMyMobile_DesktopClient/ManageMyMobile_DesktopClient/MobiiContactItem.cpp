#include "MobiiContactItem.h"

MobiiContactItem::MobiiContactItem()
{

}

MobiiContactItem::MobiiContactItem(int id, QString fn, QString ln, QString n, QDate b, QString mP, QString mPr, QString mailP, QString mailPr, QString s)
{
	this->id = id;
	this->firstname = fn;
	this->lastname = ln;
	this->mobilePerso = mP;
	this->mobilePro = mPr;
	this->society = s;
	this->notes = n;
	this->mailPerso = mailP;
	this->mailPro = mailPr;
	this->birthday = b;
}

MobiiContactItem::~MobiiContactItem()
{

}
