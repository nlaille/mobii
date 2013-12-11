#include "MobiiCalendarPanel.h"
#include "helper.h"

MobiiCalendarPanel::MobiiCalendarPanel() : eventPanel_(NULL), src_(NULL)
{
	ui.setupUi(this);

	this->firstTimeInWidget = true;
	connect(this->ui.calendar, SIGNAL(selectionChanged()), SLOT(daySelectedChanged()));
	connect(this->ui.dayEventList, SIGNAL(itemSelectionChanged()), SLOT(eventSelectedChanged()));
	connect(this->ui.newEventButton, SIGNAL(clicked()), SLOT(createNewEvent()));
	connect(this->ui.getEventButton, SIGNAL(clicked()), SLOT(requestMobileEvents()));
	connect(this->ui.updateEventButton, SIGNAL(clicked()), SLOT(updateMobileEvents()));

	this->src_ = NULL;
}

void	MobiiCalendarPanel::OnWidgetShow(void)
{
	if (this->firstTimeInWidget == true)
	{
		this->requestMobileEvents();
		this->firstTimeInWidget = false;
	}
}

void	MobiiCalendarPanel::OnWidgetClose(void)
{

}

MobiiCalendarPanel::~MobiiCalendarPanel()
{
	if (this->src_ != NULL)
		delete this->src_;
	if (this->eventPanel_ != NULL)
		delete this->eventPanel_;
	this->ui.dayEventList->clear();
}

void			MobiiCalendarPanel::OnEventBusSet(void)
{
	this->eventbus_->registerAsConsumer("update_currentEvent", this, SLOT(updateCurrentEvent(const QString, void *, void *)));
	this->eventbus_->registerAsConsumer("delete_currentEvent", this, SLOT(deleteCurrentEvent(const QString, void *, void *)));
	this->eventbus_->registerAsConsumer(MobiiEventBusHelper::eventNameFromAction("calendar"), this, SLOT(CalendarReceived(const QString, void *, void *)));
	this->eventbus_->registerAsConsumer(MobiiEventBusHelper::eventNameFromAction("valid_new_event"), this, SLOT(CalendarReceived(const QString, void *, void *)));
	this->eventbus_->registerAsConsumer(NET_MOBILE_CONNECTED, this, SLOT(OnConnected(const QString, void *, void *)));
	this->eventbus_->registerAsConsumer(NET_MOBILE_DISCONNECTED, this, SLOT(OnDisconnected(const QString, void *, void *)));
}

void			MobiiCalendarPanel::OnSessionControllerSet(void)
{
}

void		MobiiCalendarPanel::CalendarReceived(const QString eventName, void *sender, void *data)
{
	this->eventbus_->emitEvent(BLOCKING_OPERATION_END);
	// RESET BACKGROUNDS LIKE BEFORE
	if (this->src_ != NULL)
	{
		QTextCharFormat tmp = this->ui.calendar->weekdayTextFormat(Qt::Monday);
		tmp.setBackground(Qt::white);
		QDate firstdayofmonth(this->ui.calendar->selectedDate().year(), this->ui.calendar->selectedDate().month(), 1);

		while (firstdayofmonth.month() != (this->ui.calendar->selectedDate().addMonths(1).month()))
		{
			this->ui.calendar->setDateTextFormat(firstdayofmonth, tmp);
			firstdayofmonth = firstdayofmonth.addDays(1);
		}
		delete this->src_;
		this->src_ = NULL;
	}
	QVariantMap *src = reinterpret_cast<QVariantMap *>(data);
	for (int i = 0; i != (*src)["data"].toList().count(); i++)
	{
		QVariantMap data = (*src)["data"].toList()[i].toMap();
		QDateTime startDatetime = QDateTime::fromString(data["startdate"].toString(), Qt::ISODate);
		QDateTime endDateTime = QDateTime::fromString(data["enddate"].toString(), Qt::ISODate);
		QDate startDate = startDatetime.date();
		QDate endDate = endDateTime.date();

		QTextCharFormat cf = this->ui.calendar->dateTextFormat(startDate);
		cf.setBackground(Qt::red);
		this->ui.calendar->setDateTextFormat(startDate, cf);
		if (startDate != endDate)
			{
				while (startDate < endDate)
				{
					cf = this->ui.calendar->dateTextFormat(startDate);
					cf.setBackground(Qt::red);
					QDate newDate = startDate.addDays(1);
					startDate.setDate(newDate.year(), newDate.month(), newDate.day());
					this->ui.calendar->setDateTextFormat(startDate, cf);
				}
			}
	}
	this->src_ = new QList<QVariant>((*src)["data"].toList());
}

void		MobiiCalendarPanel::createNewEvent()
{
	if (this->src_ == NULL)
	{
		Helper::ExecuteInfoMsgBox("Synchroniser avant.");
		return;
	}
	this->currentEventId_ = this->src_->count();
	if (this->eventPanel_ == NULL)
	{
		this->eventPanel_ = new MobiiEventInfDialog();
		this->eventPanel_->setEventBus(this->eventbus_);
		this->eventPanel_->setStyleSheet(UI_DEFAULT_STYLE);
	}
	else
		this->eventPanel_->resetData();
	this->eventPanel_->show();
}

void		MobiiCalendarPanel::validateNewEvent(const QString eventName, void *sender, void *data)
{
	QVariantMap *newEvent = reinterpret_cast<QVariantMap *>(data);
	this->src_->append((*newEvent)["data"].toList()[0].toMap());
	this->daySelectedChanged();
}

void		MobiiCalendarPanel::updateCurrentEvent(const QString eventName, void *sender, void *data)
{
	QVariantMap *newEvent = reinterpret_cast<QVariantMap *>(data);
	QVariantMap newEventData = (*newEvent)["data"].toMap();
	
	if (!(((*newEvent)["bruteforce"].isNull() == false) && ((*newEvent)["bruteforce"].toString() == "true")))
	{
		QList<QVariant> list;
		list.append(newEventData);
		QVariantMap *datas = new QVariantMap();
		if (this->currentEventId_ == this->src_->count())
			(*datas)["action"] = "new_event";
		else
		{
			(*this->src_)[this->currentEventId_] = newEventData;
			(*datas)["action"] = "upt_events";
		}
		(*datas)["data"] = QVariant(list);
		this->eventbus_->emitEvent(JSON_SEND_REQUEST, this, datas);
	}
	else
	{
		if (this->currentEventId_ != this->src_->count())
			(*this->src_)[this->currentEventId_] = newEventData;
	}

	QVariantMap *tmp = new QVariantMap();
	(*tmp)["data"] = QVariant((*this->src_));
	this->CalendarReceived("", NULL, tmp);
	this->eventPanel_ = NULL;
	this->daySelectedChanged();
	
}

void		MobiiCalendarPanel::deleteCurrentEvent(const QString eventName, void *sender, void *data)
{
	if (this->currentEventId_ == this->src_->count())
		return;

	QList<QVariant> listevent;
	listevent.append((*this->src_)[this->currentEventId_]);
	QVariantMap *datas = new QVariantMap();
	(*datas)["action"] = "del_event";
	(*datas)["data"] = QVariant(listevent);
	this->eventbus_->emitEvent(JSON_SEND_REQUEST, this, datas);
	this->src_->removeAt(this->currentEventId_);

	QVariantMap *tmp = new QVariantMap();
	(*tmp)["data"] = QVariant((*this->src_));
	this->CalendarReceived("", NULL, tmp);
	this->eventPanel_ = NULL;
	this->daySelectedChanged();
}

void		MobiiCalendarPanel::updateMobileEvents()
{
	if (this->src_ == NULL)
	{
		Helper::ExecuteInfoMsgBox("Synchroniser avant.");
		return;
	}
	QVariantMap *data = new QVariantMap();
	(*data)["action"] = "upt_events";
	QList<QVariant> list;

	for (int i = 0; i < this->src_->count(); i++)
	{
		QVariantMap tmp = this->src_->at(i).toMap();
		if ((tmp["isModif"].isNull() == false) && (tmp["isModif"].toString() == "true"))
			list.append(this->src_[i]);
	}

	if (list.count() == 0)
	{
		Helper::ExecuteInfoMsgBox("Aucun event n'a été envoyer.");
		return;
	}
	else
		Helper::ExecuteInfoMsgBox(list.count() + " events ont été envoyés.");
	(*data)["data"] = QVariant(list);
	this->eventbus_->emitEvent(JSON_SEND_REQUEST, this, data);
}

void		MobiiCalendarPanel::requestMobileEvents()
{
	this->eventbus_->emitEvent(BLOCKING_OPERATION_BEGIN);
	QVariantMap *data = new QVariantMap();
	(*data)["action"] = "req_calendar";
	this->eventbus_->emitEvent(JSON_SEND_REQUEST, this, data);
}

void		MobiiCalendarPanel::daySelectedChanged()
{
	if (this->src_ == NULL)
		return;
	this->ui.dayEventList->clear();
	QDate currentDate = this->ui.calendar->selectedDate();

	for (int i = 0; i < this->src_->count(); i++)
	{
		QVariantMap e = (*this->src_)[i].toMap();
		QDateTime startDatetime = QDateTime::fromString(e["startdate"].toString(), Qt::ISODate);
		QDateTime endDateTime = QDateTime::fromString(e["enddate"].toString(), Qt::ISODate);
		QDate startDate = startDatetime.date();
		QDate endDate = endDateTime.date();

		if ((startDate <= currentDate) && (currentDate <= endDate))
		{
			QListWidgetItem *tmp;

			if (startDate == currentDate && endDate == currentDate)
				tmp = new QListWidgetItem(e["title"].toString() + " - De " + e["startdate"].toDateTime().time().toString() + " a " + e["enddate"].toDateTime().time().toString());
			else if (startDate == currentDate && endDate != currentDate)
				tmp = new QListWidgetItem(e["title"].toString() + " - Commence à " + e["startdate"].toDateTime().time().toString());
			else if (endDate == currentDate && startDate != currentDate)
				tmp = new QListWidgetItem(e["title"].toString() + " - Fini à " + e["enddate"].toDateTime().time().toString());
			else
				tmp = new QListWidgetItem(e["title"].toString() + " - Toute la journée");

			if (e["icon"].toString().count() != 0)
				tmp->setIcon(QIcon(":/icons/Resources/" + e["icon"].toString() + ".png"));
			else
				tmp->setIcon(QIcon(":/icons/Resources/cake.png"));
			QFont f("Segoe UI");
			f.setPixelSize(20);
			tmp->setFont(f);
			// CHEAT
			tmp->setData(667, i);
			tmp->setData(666, e);
			this->ui.dayEventList->addItem(tmp);
		}		
	}
}

void		MobiiCalendarPanel::eventSelectedChanged()
{
	//CHEAT
	if (this->ui.dayEventList->selectedItems().size() != 0)
	{
		QVariant e = this->ui.dayEventList->selectedItems()[0]->data(666);
		this->currentEventId_ = this->ui.dayEventList->selectedItems()[0]->data(667).toInt();
		QVariantMap *tmp = &this->src_->at(this->currentEventId_).toMap();
		(*tmp)["isModif"] = "true";
		if (this->eventPanel_ == NULL)
		{
			this->eventPanel_ = new MobiiEventInfDialog(e.toMap());
			this->eventPanel_->setEventBus(this->eventbus_);
			this->eventPanel_->setStyleSheet(UI_DEFAULT_STYLE);
		}
		else
			this->eventPanel_->setData(e.toMap());
		this->eventPanel_->show();
	}
	else
	{
		if (this->eventPanel_ != NULL)
		{
			this->eventPanel_->close();
			this->eventPanel_ = NULL;
		}
	}
}

void		MobiiCalendarPanel::OnDisconnected(const QString eventName, void *, void *)
{
	qDebug() << "OnDisconnected MobiiCalendarPanel ENTRER";
	this->eventbus_->emitEvent(BLOCKING_OPERATION_END);
	if (this->eventPanel_ != NULL)
		this->eventPanel_->close();
	if (this->src_ != NULL)
	{
		for (int i = 0; i != this->src_->count(); i++)
		{
			QVariantMap data = (*this->src_)[i].toMap();
			this->rawCalendarDatas_.append(data);
		}
	}
	// Save calendar table to the device data file
	DeviceEntity *device = this->sessioncon_->currentDevice();
	if (device == NULL)
	{
		qDebug() << "ERROR : Device == NULL";
		return;
	}
	device->writeDatasForModule<MobiiCalendarPanel::TCalendarBackupContainer>("calendar", this->rawCalendarDatas_);
	this->rawCalendarDatas_.clear();

	if (this->src_ != NULL)
	{
		delete this->src_;
		this->src_ = NULL;
	}
	if (this->eventPanel_ != NULL)
		this->eventPanel_ = NULL;
	this->ui.dayEventList->clear();
	qDebug() << "OnDisconnected MobiiCalendarPanel SORTIE";
}

void			MobiiCalendarPanel::OnConnected(const QString eventName, void *sender, void *data)
{
	// Loads calendar stored from previous session
	DeviceEntity *device = this->sessioncon_->currentDevice();
	if (device == NULL)
	{
		qDebug() << "ERROR : Device == NULL";
		return;
	}
	MobiiCalendarPanel::TCalendarBackupContainer *backup = device->readDatasForModule<MobiiCalendarPanel::TCalendarBackupContainer>("calendar");

	if (backup != NULL) // NULL means that there is no backup yet
	{
		qDebug() << "I found" << backup->count() << " events in the backup file!";
		QVariantMap *newEvents = new QVariantMap();

		(*newEvents)["data"] = QVariant(*backup);
		this->CalendarReceived("", NULL, newEvents);
		delete newEvents;
	}
}