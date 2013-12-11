#include <qevent.h>
#include "MobiiEventInfDialog.h"
#include "helper.h"

MobiiEventInfDialog::MobiiEventInfDialog()
{
	this->setModal(false);
	ui.setupUi(this);
	
	connect(this->ui.deleteEventButton, SIGNAL(clicked()), this, SLOT(deleteCurrentEvent()));
	connect(this->ui.updateEventButton, SIGNAL(clicked()), this, SLOT(updateCurrentEvent()));
	connect(this->ui.dateStart, SIGNAL(dateTimeChanged(QDateTime)), this, SLOT(startTimeChanged(QDateTime)));
	connect(this->ui.dateEnd, SIGNAL(dateTimeChanged(QDateTime)), this, SLOT(endTimeChanged(QDateTime)));

	this->ui.dateStart->setDateTime(QDateTime::currentDateTime());
	this->ui.dateEnd->setDateTime(QDateTime::currentDateTime().addSecs(3600));
}

MobiiEventInfDialog::~MobiiEventInfDialog()
{

}

/// Specifies actions to perform when the event bus is set.
void		MobiiEventInfDialog::OnEventBusSet(void)
{
}
/// Specifies actions to perform when the session controller is set.
void		MobiiEventInfDialog::OnSessionControllerSet(void)
{
}

MobiiEventInfDialog::MobiiEventInfDialog(QVariantMap& eventData)
{
	this->setModal(false);
	ui.setupUi(this);

	connect(this->ui.deleteEventButton, SIGNAL(clicked()), this, SLOT(deleteCurrentEvent()));
	connect(this->ui.updateEventButton, SIGNAL(clicked()), this, SLOT(updateCurrentEvent()));
	connect(this->ui.dateStart, SIGNAL(dateTimeChanged(QDateTime)), this, SLOT(startTimeChanged(QDateTime)));
	connect(this->ui.dateEnd, SIGNAL(dateTimeChanged(QDateTime)), this, SLOT(endTimeChanged(QDateTime)));

	this->currentID_ = eventData["id"].toString();
	this->ui.noteEdit->setText(eventData["notes"].toString());
	this->ui.whereEdit->setText(eventData["where"].toString());
	this->ui.nameEdit->setText(eventData["title"].toString());
	this->ui.dateEnd->setDateTime(QDateTime::fromString(eventData["enddate"].toString(), Qt::ISODate));
	this->ui.dateStart->setDateTime(QDateTime::fromString(eventData["startdate"].toString(), Qt::ISODate));

	if (eventData["icon"].toString().size() != 0)
	{
		for (int i = 0; i != this->ui.iconBox->count(); i++)
		{
			if (this->ui.iconBox->itemText(i).compare(eventData["icon"].toString()) == 0)
			{
				this->ui.iconBox->setCurrentIndex(i);
				break;
			}
		}
	}
}

void		MobiiEventInfDialog::startTimeChanged(QTime newtime)
{
	if (newtime > this->ui.dateEnd->time())
	{
		QMessageBox tmp;
		tmp.setText("L'heure de debut de l'evenement doit etre superieur a celle de fin");
		tmp.exec();
	}
	else
		this->ui.dateStart->setTime(newtime);
}

void		MobiiEventInfDialog::endTimeChanged(QTime newtime)
{
	if (newtime < this->ui.dateStart->time())
	{
		QMessageBox tmp;
		tmp.setText("L'heure de fin de l'evenement doit etre inferieur a celle de début");
		tmp.exec();
	}
	else
		this->ui.dateEnd->setTime(newtime);
}

void		MobiiEventInfDialog::updateCurrentEvent()
{
	QVariantMap *datas = new QVariantMap();
	QVariantMap *data = new QVariantMap();

	(*data)["isModif"] = "false";
	(*data)["id"] = this->currentID_;
	(*data)["notes"] = this->ui.noteEdit->toPlainText();
	(*data)["where"] = this->ui.whereEdit->text();
	(*data)["title"] = this->ui.nameEdit->text();
	(*data)["enddate"] = Helper::ToStringISODateWithTimeZone(this->ui.dateEnd->dateTime());
	(*data)["startdate"] = Helper::ToStringISODateWithTimeZone(this->ui.dateStart->dateTime());
	(*data)["icon"] = this->ui.iconBox->currentText();
	(*datas)["data"] = QVariant(*data);
	this->eventbus_->emitEvent("update_currentEvent", this, datas);
	this->close();
}

void	MobiiEventInfDialog::closeEvent(QCloseEvent *evt)
{
	QVariantMap *datas = new QVariantMap();
	QVariantMap *data = new QVariantMap();

	(*data)["isModif"] = "true";
	(*data)["id"] = this->currentID_;
	(*data)["notes"] = this->ui.noteEdit->toPlainText();
	(*data)["where"] = this->ui.whereEdit->text();
	(*data)["title"] = this->ui.nameEdit->text();
	(*data)["enddate"] = Helper::ToStringISODateWithTimeZone(this->ui.dateEnd->dateTime());
	(*data)["startdate"] = Helper::ToStringISODateWithTimeZone(this->ui.dateStart->dateTime());
	(*data)["icon"] = this->ui.iconBox->currentText();

	(*datas)["data"] = QVariant(*data);
	(*datas)["bruteforce"] = "true";
	this->eventbus_->emitEvent("update_currentEvent", this, datas);
	
	evt->accept();
}

void		MobiiEventInfDialog::deleteCurrentEvent()
{
	QVariantMap *datas = new QVariantMap();

	this->eventbus_->emitEvent("delete_currentEvent", this, datas);
	this->close();
}

void		MobiiEventInfDialog::setData(QVariantMap& eventData)
{
	this->currentID_ = eventData["id"].toString();
	this->ui.noteEdit->setText(eventData["notes"].toString());
	this->ui.whereEdit->setText(eventData["where"].toString());
	this->ui.nameEdit->setText(eventData["title"].toString());
	this->ui.dateEnd->setDateTime(eventData["enddate"].toDateTime());
	this->ui.dateStart->setDateTime(eventData["startdate"].toDateTime());
	
	if (eventData["icon"].toString().size() != 0)
	{
		for (int i = 0; i != this->ui.iconBox->count(); i++)
		{
			if (this->ui.iconBox->itemText(i).compare(eventData["icon"].toString()) == 0)
			{
				this->ui.iconBox->setCurrentIndex(i);
				break;
			}
		}
	}

}

void		MobiiEventInfDialog::resetData()
{
	this->currentID_ = "";
	this->ui.dateStart->setDateTime(QDateTime::currentDateTime());
	this->ui.dateEnd->setDateTime(QDateTime::currentDateTime().addSecs(3600));
	this->ui.nameEdit->setText("");
	this->ui.noteEdit->setText("");
	this->ui.whereEdit->setText("");
}