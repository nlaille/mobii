#include		"MobiiSmsStreamItem.h"


MobiiSmsStreamItem::MobiiSmsStreamItem(
		const QString &senderNb,
		const QString &lastMessage, 
		const QDateTime &lastDate)
{
	this->ui.setupUi(this);

	this->setSenderNb(senderNb);
	this->setLastMessage(lastMessage);
	this->setLastDate(lastDate);
}


MobiiSmsStreamItem::~MobiiSmsStreamItem(void)
{
}


void		MobiiSmsStreamItem::setSenderNb(const QString &senderNb)
{
	this->m_senderNb = senderNb;
	this->ui.senderText->setText(senderNb);
}


void		MobiiSmsStreamItem::setLastMessage(const QString &message)
{
	// Adds an ellipsis (...) at the end if the text is too long
	QFont font = this->ui.lastMessageText->font();
	QFontMetrics fm = QFontMetrics(font);
	QString ellidedStr = fm.elidedText(message, Qt::ElideRight, this->ui.lastMessageText->width());
	this->ui.lastMessageText->setText(ellidedStr);
}


void		MobiiSmsStreamItem::setLastDate(const QDateTime &date)
{
	this->ui.dateText->setText(date.toString("dd/MM/yyyy") + " à " + date.toString("hh:mm:ss"));
}


void		MobiiSmsStreamItem::setConvenientSenderName(const QString &name)
{
	this->ui.senderText->setText(name);
}


const QString &MobiiSmsStreamItem::senderNb(void) const
{
	return this->m_senderNb;
}


const QString MobiiSmsStreamItem::senderText(void) const
{
	return this->ui.senderText->text();
}