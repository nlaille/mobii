#include		<qheaderview.h>
#include		<qlistwidget.h>
#include		<qdatetime.h>
#include		<qdebug.h>
#include		<qmenu.h>

#include		"ConfigurationFileIo.h"
#include		"MobiiEventBusCodes.h"
#include		"MobiiSmsPanelForm.h"
#include		"MobiiSmsBulletItem.h"
#include		"MobiiSmsStreamItem.h"
#include		"ui_MobiiSmsLoadMoreItem.h"


MobiiSmsPanelForm::MobiiSmsPanelForm(void)
	: m_newSmsDialog(NULL), m_streamOffset(0), m_synchronizeOnNextTabSelection(true)
{
	this->ui.setupUi(this);
	
	this->ui.discussionStream->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	this->ui.discussionStream->setContextMenuPolicy(Qt::CustomContextMenu);

	connect(this->ui.retrieveSmsButton, SIGNAL(clicked()), this, SLOT(OnRetrieveSmsClicked()));
	connect(this->ui.createSmsButton, SIGNAL(clicked()), this, SLOT(OnWriteNewSmsClicked()));
	connect(this->ui.streamsList, SIGNAL(currentItemChanged(QListWidgetItem *, QListWidgetItem *)), this, SLOT(OnStreamSelectionChanged(QListWidgetItem *, QListWidgetItem *)));
	connect(this->ui.discussionStream, SIGNAL(cellClicked(int, int)), this, SLOT(OnDiscussionStreamItemClicked(int, int)));
	connect(this->ui.discussionStream, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(OnDiscussionStreamContextMenu(const QPoint &)));
	connect(this->ui.sendReplyBtn, SIGNAL(clicked()), this, SLOT(OnReplyBtnClicked()));
	connect(this->ui.smsAnswerField, SIGNAL(returnPressed()), this, SLOT(OnReplyBtnClicked()));
}


MobiiSmsPanelForm::~MobiiSmsPanelForm(void)
{
}


void			MobiiSmsPanelForm::OnWidgetShow(void)
{
	if (this->m_synchronizeOnNextTabSelection == true)
		this->OnRetrieveSmsClicked();
	this->m_synchronizeOnNextTabSelection = false;
}


void			MobiiSmsPanelForm::OnEventBusSet(void)
{
	this->eventbus_->registerAsConsumer(MobiiEventBusHelper::eventNameFromAction("sms"), this, SLOT(OnSmsReceived(const QString, void *, void *)));
	this->eventbus_->registerAsConsumer(NET_MOBILE_CONNECTED, this, SLOT(OnConnected(const QString, void *, void *)));
	this->eventbus_->registerAsConsumer(NET_MOBILE_DISCONNECTED, this, SLOT(OnDisconnected(const QString, void *, void *)));
	this->eventbus_->registerAsConsumer(CONTACT_NAME_FOR_PHONENB, this, SLOT(OnPhoneNbMatchFound(const QString, void *, void *)));
}


void			MobiiSmsPanelForm::OnRetrieveSmsClicked(void)
{
	this->ui.retrieveSmsButton->setEnabled(false);
	QVariantMap *data = new QVariantMap();
	(*data)["action"] = "req_sms";
	(*data)["from"] = this->m_lastReceivedDate.toString(Qt::ISODate);
	this->eventbus_->emitEvent(JSON_SEND_REQUEST, this, data);
}


void			MobiiSmsPanelForm::OnWriteNewSmsClicked(void)
{
	this->m_newSmsDialog = new MobiiNewSmsDialog(this->eventbus_);
	this->m_newSmsDialog->exec();
	delete this->m_newSmsDialog;
}


QString			MobiiSmsPanelForm::getUniformizedSenderNb(const QString &nb)
{
	QString tmp = nb.trimmed();
	tmp.replace('+', 'p');
	tmp.replace('#', 's');
	tmp.replace('*', 'a');
	tmp.replace('.', 'd');
	tmp.replace('(', '<');
	tmp.replace(')', '>');
	return tmp;
}


QList<QString>	MobiiSmsPanelForm::getListOfSendersOrderedByLastReceived(QList<QString> list)
{
	if (list.count() <= 1)
		return list;
	QString oldestSender = *(list.begin());

	// Find the latest date amoung the senders
	for (QList<QString>::iterator it = list.begin(); it != list.end(); ++it)
	{
		if (this->m_streams[*it].first()[C_ItemDateIdx].toDateTime() > this->m_streams[oldestSender].first()[C_ItemDateIdx].toDateTime())
			oldestSender = *it;
	}

	// Reprocess the list without the latest element
	list.removeOne(oldestSender);
	QList<QString> res;
	res.push_back(oldestSender);
	res.append(this->getListOfSendersOrderedByLastReceived(list));
	return res;
}


void			MobiiSmsPanelForm::updateStreamsListOnUi(void)
{
	// Clear the UI first
	this->ui.discussionStream->clearContents();
	this->ui.discussionStream->setRowCount(0);
	this->ui.streamsList->clear();

	if (this->m_streams.size() == 0)
		return;

	// Display streams list ordered by last received date
	QList<QString> senders = this->getListOfSendersOrderedByLastReceived(this->m_streams.keys());
	QMap<QString, QString> phoneNbsToMatch;
	for (QList<QString>::iterator it = senders.begin(); it != senders.end(); ++it)
	{
		// Assume that the first item of TSmsItemList is the latest message, as messages are supposed to be sorted by date upon reception
		TSmsItem &lastReceivedMsg = this->m_streams[*it].first();
		MobiiSmsStreamItem *customItem = new MobiiSmsStreamItem(*it, lastReceivedMsg[C_ItemContentIdx].toString(), lastReceivedMsg[C_ItemDateIdx].toDateTime());
		phoneNbsToMatch.insert(*it, "");

		// Create UI row and its underlying model
		QListWidgetItem *item = new QListWidgetItem();
		QSize itemSize = item->sizeHint();
		itemSize.setHeight(90);
		item->setSizeHint(itemSize);
		this->ui.streamsList->addItem(item);
		this->ui.streamsList->setItemWidget(item, customItem);
	}

	// Request for convenient names match
	this->eventbus_->emitEvent(CONTACT_PHONENB_FOR_NAME, this, &phoneNbsToMatch);

	// If a stream was displayed while streams update occured, update the stream on UI
	if (!this->m_currentSenderDisplayed.isEmpty())
	{
		this->ui.discussionStream->clearContents();
		this->ui.discussionStream->setRowCount(0);
		this->m_streamOffset = 0;
		this->displayStreamForSenderOnUi();
	}
}


void			MobiiSmsPanelForm::displayStreamForSenderOnUi(void)
{
	ConfigurationFileIo cfg;

	this->ui.discussionStream->setCursor(Qt::WaitCursor);
	TSmsItemList &list = this->m_streams[this->m_currentSenderDisplayed];
	quint32 i;
	for (i = 0; i < cfg.smsPanelNbOfMessagesToDisplay() && i + this->m_streamOffset < list.size(); ++i)
	{
		TSmsItem &smscontent = list[this->m_streamOffset + i];
		MobiiSmsBulletItem *bullet = new MobiiSmsBulletItem(smscontent[C_ItemContentIdx].toString(), smscontent[C_ItemDateIdx].toDateTime());
		this->ui.discussionStream->insertRow(this->ui.discussionStream->rowCount());
		QTableWidgetItem *item = new QTableWidgetItem();
		int column = (smscontent[C_ItemSentByPhoneIdx].toBool() == true ? 0 : 1);
		bullet->setColorMode(column);
		this->ui.discussionStream->setItem(this->ui.discussionStream->rowCount() - 1, column, item);
		this->ui.discussionStream->setCellWidget(this->ui.discussionStream->rowCount() - 1, column, bullet);
		this->ui.discussionStream->resizeColumnsToContents();
		this->ui.discussionStream->resizeRowsToContents();
	}
	this->m_streamOffset += i;

	// Display a `load more' button at the bottom if end of list not reached
	if (this->m_streamOffset < list.size())
		this->makeLoadMoreButton();
	this->ui.discussionStream->setCursor(Qt::ArrowCursor);
}


void			MobiiSmsPanelForm::makeLoadMoreButton(void)
{
	Ui::MobiiSmsLoadMoreItem *loadmore = new Ui::MobiiSmsLoadMoreItem();
	QWidget *widget = new QWidget();
	loadmore->setupUi(widget);

	this->ui.discussionStream->insertRow(this->ui.discussionStream->rowCount());
	QTableWidgetItem *item = new QTableWidgetItem();
	this->ui.discussionStream->setItem(this->ui.discussionStream->rowCount() - 1, 0, item);
	this->ui.discussionStream->setSpan(this->ui.discussionStream->rowCount() - 1, 0, 1, 2);
	this->ui.discussionStream->setCellWidget(this->ui.discussionStream->rowCount() - 1, 0, widget);
}


void			MobiiSmsPanelForm::OnSmsReceived(
	const QString eventName, 
	void *sender, 
	void *data)
{
	this->eventbus_->emitEvent(BLOCKING_OPERATION_BEGIN);

	// Insert the new SMS in the internal model
	QList<QString> affectedSenders;
	QVariantMap *src = reinterpret_cast<QVariantMap *>(data);
	if ((*src).find("list") != (*src).end())
	{
		QVariantList allMessages = (*src)["list"].toList();
		for (QVariantList::iterator it = allMessages.begin(); it != allMessages.end(); ++it)
		{
			// Creates the internal model depending on the contents of the SMS received
			QVariantMap oneMsg = (*it).toMap();
			QString sender = oneMsg["sender"].toString();
			TSmsItemList &itemList = this->m_streams[sender];
			TSmsItem newItem;
			newItem.insert(C_ItemContentIdx, oneMsg["content"].toString());
			newItem.insert(C_ItemDateIdx, oneMsg["date"].toDateTime());
			newItem.insert(C_ItemSentByPhoneIdx, oneMsg["sent_by_phone"].toBool());
			itemList.push_back(newItem);

			// This sender's list shall be sorted later
			if (!affectedSenders.contains(sender))
				affectedSenders.push_back(sender);

			// Resets the latest date of reception for further synchronization
			if (oneMsg["date"].toDateTime() > this->m_lastReceivedDate)
				this->m_lastReceivedDate = oneMsg["date"].toDateTime();
		}
	}
	else
		qDebug() << Q_FUNC_INFO << "Invalid incoming SMS list, no `list' key found.";

	// Sort all results: from newest to oldest
	for (QList<QString>::iterator it = affectedSenders.begin(); it != affectedSenders.end(); ++it)
		qStableSort(this->m_streams[*it].begin(), this->m_streams[*it].end(), &MobiiSmsPanelForm::smsDateComparison);

	// Update UI
	this->updateStreamsListOnUi();
	this->ui.retrieveSmsButton->setEnabled(true);

	// Save SMS table to the device data file
	DeviceEntity *device = this->sessioncon_->currentDevice();
	if (device == NULL)
		return;
	device->writeDatasForModule<TSmsSenderList>("sms", this->m_streams);
	this->setDateOfLastMessage();

	// Notify the synchronization status
	QMap<QString, QString> args;
	args["emitOnClick"] = UI_CHANGE_ACTIVE_PANEL;
	args["emitOnClickData"] = "TopSmsPanelButton";
	if ((*src).find("list") != (*src).end())
	{
		QVariantList allMessages = (*src)["list"].toList();
		args["title"] = "Synchronisation des SMS terminée";
		args["msg"] = (allMessages.count() == 0 ? "Aucun" : QString::number(allMessages.count()));
		args["msg"] += " message(s) récupéré(s).";
		if (allMessages.count() > 0)
		{
			args["msg"] += "\nDernier message en date du ";
			args["msg"] += this->m_lastReceivedDate.toString("dd/MM/yyyy") + QString(" à ") + this->m_lastReceivedDate.toString("hh:mm:ss");
		}
		args["icon"] = "info";
	}
	else
	{
		args["title"] = "Erreur de synchronisation";
		args["msg"] = "Une erreur s'est produite lors de la synchronisation de vos SMS.";
		args["icon"] = "warn";
	}
	this->eventbus_->emitEvent(DISPLAY_NOTIFICATION_SYSTRAY, this, &args);
	delete data;

	this->eventbus_->emitEvent(BLOCKING_OPERATION_END);
}


void			MobiiSmsPanelForm::OnConnected(const QString eventName, void *sender, void *data)
{
	// Clear the UI first
	this->ui.discussionStream->clearContents();
	this->ui.discussionStream->setRowCount(0);
	this->ui.streamsList->clear();

	// Loads SMS stored from previous session
	DeviceEntity *device = this->sessioncon_->currentDevice();
	if (device == NULL)
		return;
	TSmsSenderList *backup = device->readDatasForModule<TSmsSenderList>("sms");
	if (backup != NULL) // NULL means that there is no backup yet
	{
		this->m_streams = *backup;
		delete backup;
	}
	this->m_lastReceivedDate = getDateOfLastMessage();

	// Update UI
	this->updateStreamsListOnUi();
	this->ui.retrieveSmsButton->setEnabled(true);
}


void			MobiiSmsPanelForm::OnDisconnected(const QString eventName, void *sender, void *data)
{
	this->ui.discussionStream->clearContents();
	this->ui.discussionStream->setRowCount(0);
	this->ui.streamsList->clear();
	this->ui.smsAnswerField->setEnabled(false);
	this->ui.sendReplyBtn->setEnabled(false);
	this->m_synchronizeOnNextTabSelection = true;
}


void			MobiiSmsPanelForm::OnDiscussionStreamItemClicked(int row, int column)
{
	QWidget *widget = this->ui.discussionStream->cellWidget(row, column);
	if (widget != NULL && widget->objectName() == "MobiiSmsLoadMoreItem")
	{
		// Remove the `load more' button
		this->ui.discussionStream->removeRow(row);

		// Display the next n messages
		this->displayStreamForSenderOnUi();
	}
}


void			MobiiSmsPanelForm::OnDiscussionStreamContextMenu(const QPoint &point)
{
	QTableWidgetItem *item = this->ui.discussionStream->itemAt(point);
	if (item == NULL)
		return;
	QWidget *widget = this->ui.discussionStream->cellWidget(item->row(), item->column());
	if (widget != NULL && widget->objectName() == "SmsBulletItem")
	{
		QMenu *menu = new QMenu("Message");
		menu->addAction(QIcon(":/buttons/Resources/duplicate.png"), "Copier le message", this, SLOT(OnCopyMessageRequested(bool)));
		menu->popup(this->ui.discussionStream->mapToGlobal(point));
	}
}


void			MobiiSmsPanelForm::OnCopyMessageRequested(bool)
{
	QList<QTableWidgetItem *>selected = this->ui.discussionStream->selectedItems();
	if (selected.size() != 1)
		return;
	QTableWidgetItem *item = selected.front();
	MobiiSmsBulletItem *widget = dynamic_cast<MobiiSmsBulletItem *>(this->ui.discussionStream->cellWidget(item->row(), item->column()));
	if (widget == NULL)
		return;

	QClipboard *clipboard = qApp->clipboard();
	QString s;
	if (item->column() == 0)
		s = "Envoyé à " + this->m_currentSenderDisplayed;
	else
		s = this->m_currentSenderDisplayed + " a envoyé";
	s += " le " + widget->datetime() + " : " + widget->content();
	clipboard->setText(s);
}


void			MobiiSmsPanelForm::OnStreamSelectionChanged(QListWidgetItem *curr, QListWidgetItem *prev)
{
	MobiiSmsStreamItem *streamTitle = dynamic_cast<MobiiSmsStreamItem *>(this->ui.streamsList->itemWidget(curr));
	if (streamTitle == NULL)
	{
		qDebug() << Q_FUNC_INFO << "Item selected not a valid MobiiSmsStreamItem.";
		return;
	}
	this->ui.discussionStream->clearContents();
	this->ui.discussionStream->setRowCount(0);
	this->m_streamOffset = 0;

	this->m_currentSenderDisplayed = streamTitle->senderNb();
	this->ui.senderLbl->setText(QString("Conversation avec ") + streamTitle->senderText());
	this->displayStreamForSenderOnUi();

	this->ui.smsAnswerField->setEnabled(true);
	this->ui.sendReplyBtn->setEnabled(true);
}


void			MobiiSmsPanelForm::OnPhoneNbMatchFound(const QString eventName, void *sender, void *data)
{
	QMap<QString, QString> *matches = (QMap<QString, QString> *)data;
	for (int i = 0; i < this->ui.streamsList->count(); ++i)
	{
		QListWidgetItem *item = this->ui.streamsList->item(i);
		MobiiSmsStreamItem *entity = dynamic_cast<MobiiSmsStreamItem *>(this->ui.streamsList->itemWidget(item));
		if (entity == NULL)
		{
			qDebug() << "Found an entity which is not a MobiiSmsStreamItem in the streams list.";
			return;
		}
		if (matches->contains(entity->senderNb()))
		{
			QString convenient = (*matches)[entity->senderNb()];
			if (convenient == "")
				continue;
			entity->setConvenientSenderName(convenient);
			qDebug() << "Convenient name for" << entity->senderNb() << "which is" << convenient;
		}
	}
}


void			MobiiSmsPanelForm::OnReplyBtnClicked(void)
{
	if (this->ui.smsAnswerField->text().length() == 0)
		return;

	// Send JSON request
	QVariantMap *data = new QVariantMap();
	(*data)["action"] = "req_sendsms";
	(*data)["dest"] = this->m_currentSenderDisplayed;
	(*data)["body"] = this->ui.smsAnswerField->text();
	this->eventbus_->emitEvent(JSON_SEND_REQUEST, this, data);

	// Clear the andwer field
	this->ui.smsAnswerField->clear();
}


QDateTime		MobiiSmsPanelForm::getDateOfLastMessage(void)
{
	DeviceEntity *device = this->sessioncon_->currentDevice();
	if (device == NULL)
		return QDateTime::fromMSecsSinceEpoch(0);
	QDateTime *output = device->readDatasForModule<QDateTime>("sms_sync_token");
	if (output == NULL)
		return QDateTime::fromMSecsSinceEpoch(0);
	return *output;
}


void			MobiiSmsPanelForm::setDateOfLastMessage(void)
{
	DeviceEntity *device = this->sessioncon_->currentDevice();
	if (device == NULL)
		return;
	device->writeDatasForModule<QDateTime>("sms_sync_token", this->m_lastReceivedDate);
}


bool			MobiiSmsPanelForm::smsDateComparison(const TSmsItem &a, const TSmsItem &b)
{
	return a[C_ItemDateIdx].toDateTime() > b[C_ItemDateIdx].toDateTime();
}