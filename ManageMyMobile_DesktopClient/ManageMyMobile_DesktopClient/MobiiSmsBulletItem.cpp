#include		"MobiiSmsBulletItem.h"


const QString C_SMSBULLETSTYLE_RECP = "QWidget{font-family: \"Segoe UI\"}#SmsContainer {background-color:#6BE554;border-radius:24px}QLabel {background-color: rgba(255, 255, 255, 0);color:#fff}";
const QString C_SMSBULLETSTYLE_SENDER = "QWidget{font-family: \"Segoe UI\"}#SmsContainer {background-color:#E5E4E9;border-radius:24px}QLabel {background-color: rgba(255, 255, 255, 0)}";

MobiiSmsBulletItem::MobiiSmsBulletItem(void)
{
	this->ui.setupUi(this);
}


MobiiSmsBulletItem::MobiiSmsBulletItem(const QString &content, const QDateTime &datetime)
{
	this->ui.setupUi(this);
	this->setContent(content);
	this->setDatetime(datetime);
	this->ui.content->adjustSize();
	this->ui.SmsContainer->adjustSize();
}


MobiiSmsBulletItem::~MobiiSmsBulletItem(void)
{
}


const QString		MobiiSmsBulletItem::content(void) const
{
	return this->ui.content->text();
}


const QString		MobiiSmsBulletItem::datetime(void) const
{
	return this->ui.date->text();
}


void				MobiiSmsBulletItem::setContent(const QString &content)
{
	this->ui.content->setText(content);
	this->ui.content->adjustSize();
	this->ui.SmsContainer->adjustSize();
}


void				MobiiSmsBulletItem::setDatetime(const QDateTime &dtime)
{
	this->ui.date->setText(dtime.toString("dd/MM/yyyy") + " à " + dtime.toString("hh:mm:ss"));
}


void				MobiiSmsBulletItem::setColorMode(int mode)
{
	switch (mode)
	{
	case (0):
		this->ui.SmsContainer->setStyleSheet(C_SMSBULLETSTYLE_SENDER);
		break;
	case (1):
		this->ui.SmsContainer->setStyleSheet(C_SMSBULLETSTYLE_RECP);
		break;
	}
}