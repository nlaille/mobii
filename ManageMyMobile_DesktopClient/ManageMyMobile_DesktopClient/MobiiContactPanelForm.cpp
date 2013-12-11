#include "MobiiContactPanelForm.h"
#include "helper.h"


MobiiContactPanelForm::MobiiContactPanelForm()
	: contactPanel_(NULL), src_(NULL), currentContactId_(0)
{
	ui.setupUi(this);

	this->firstTimeInWidget = true;
	this->ui.searchContact->setText("");
	connect(this->ui.sendContactsToMobile, SIGNAL(clicked()), this, SLOT(SendContactsToMobile()));
	connect(this->ui.getContactButton, SIGNAL(clicked()), this, SLOT(RetrieveContactsClicked()));
	connect(this->ui.contactContent->findChild<QListWidget*>("listWidget"), SIGNAL(itemSelectionChanged()), this, SLOT(contactSelected()));
	connect(this->ui.searchContact, SIGNAL(textChanged(QString)), this, SLOT(updateContactList(QString)));
	connect(this->ui.newContact, SIGNAL(clicked()), this, SLOT(newContact()));

	QDir profileImgDir(this->config_.userStorageDir() + "/ProfileImages");

	if (profileImgDir.exists() == false)
		profileImgDir.mkdir(this->config_.userStorageDir() + "/ProfileImages");
	
	this->src_ = NULL;
	/*QFile tmp("C:/Users/Mycose/Desktop/default_profile.jpg");

	tmp.open(QIODevice::ReadOnly);

	if (tmp.exists() == true)
	{
		QByteArray tab = tmp.readAll();
		QByteArray base64Encoded  = tab.toBase64();  

		QVariantMap *tmp = new QVariantMap();	
		(*tmp)["name"] = "test.jpg";
		(*tmp)["data"] = base64Encoded;
		(*tmp)["size"] = tab.size();
		(*tmp)["type"] = "PhonePic";

		this->eventbus_->emitEvent(JSON_SEND_REQUEST, this, tmp);
		//this->fileReceived("", "", tmp);
	}*/
}

void	MobiiContactPanelForm::OnWidgetShow(void)
{
	if (this->firstTimeInWidget == true)
	{
		this->RetrieveContactsClicked();
		this->firstTimeInWidget = false;
	}
}

void	MobiiContactPanelForm::OnWidgetClose(void)
{

}

MobiiContactPanelForm::~MobiiContactPanelForm()
{
	qDebug() << "Destructeur start ContactPanelForm";
	if (this->src_ != NULL)
		delete this->src_;
	if (this->contactPanel_ != NULL)
		this->contactPanel_ = NULL;
	qDebug() << "Destructeur end ContactPanelForm";
}

void			MobiiContactPanelForm::OnEventBusSet(void)
{
	this->eventbus_->registerAsConsumer(MobiiEventBusHelper::eventNameFromAction("file"), this, SLOT(fileReceived(const QString, void *, void *)));
	this->eventbus_->registerAsConsumer("update_currentContact", this, SLOT(updateCurrentContact(const QString, void *, void *)));
	this->eventbus_->registerAsConsumer("delete_currentContact", this, SLOT(deleteCurrentContact(const QString, void *, void *)));
	this->eventbus_->registerAsConsumer(MobiiEventBusHelper::eventNameFromAction("valid_new_contact"), this, SLOT(validateNewContact(const QString, void *, void *)));
	this->eventbus_->registerAsConsumer(MobiiEventBusHelper::eventNameFromAction("contacts"), this, SLOT(ContactsReceived(const QString, void *, void *)));
	this->eventbus_->registerAsConsumer(NET_MOBILE_CONNECTED, this, SLOT(OnConnected(const QString, void *, void *)));
	this->eventbus_->registerAsConsumer(NET_MOBILE_DISCONNECTED, this, SLOT(OnDisconnected(const QString, void *, void *)));
	this->eventbus_->registerAsConsumer(CONTACT_PHONENB_FOR_NAME, this, SLOT(findNameWithPhone(const QString, void *, void *)));
}

void			MobiiContactPanelForm::OnSessionControllerSet(void)
{
}

void			MobiiContactPanelForm::resetContactList()
{
	QListWidget *contactList = this->ui.contactContent->findChild<QListWidget*>("listWidget");

	this->ui.searchContact->setText("");
	contactList->clear();
	for (int i = 0; i < this->src_->count(); i++)
	{
		QVariantMap data = this->src_->at(i).toMap();
		QListWidgetItem *n = new QListWidgetItem(data["firstname"].toString() + " " + data["lastname"].toString());	
		contactList->addItem(n);
	}
	contactList->sortItems();
}

void			MobiiContactPanelForm::updateContactList(QString str)
{
	QListWidget *contactList = this->ui.contactContent->findChild<QListWidget*>("listWidget");

	for (int i = 0; i < contactList->count(); i++)
	{
		if (str.size() == 0)
			contactList->item(i)->setHidden(false);
		else if (contactList->item(i)->text().toLower().contains(str.toLower()) == false)
			contactList->item(i)->setHidden(true);
		else
			contactList->item(i)->setHidden(false);
	}
}

void					MobiiContactPanelForm::deleteCurrentContact(const QString eventName, void *eventEmitter, void *data)
{
	if (this->currentContactId_ == this->src_->count())
		return;

	QList<QVariant> listcontact;
	listcontact.append((*this->src_)[this->currentContactId_]);
	QVariantMap *datas = new QVariantMap();
	(*datas)["action"] = "del_contact";
	(*datas)["data"] = QVariant(listcontact);
	this->eventbus_->emitEvent(JSON_SEND_REQUEST, this, datas);

	this->src_->removeAt(this->currentContactId_);

	this->contactPanel_ = NULL;
	this->resetContactList();
}

void					MobiiContactPanelForm::validateNewContact(const QString eventName, void *eventEmitter, void *data)
{
	QVariantMap *d = reinterpret_cast<QVariantMap *>(data);
	this->src_->append((*d)["data"].toList()[0].toMap());
	this->resetContactList();
}

void					MobiiContactPanelForm::updateCurrentContact(const QString eventName, void *eventEmitter, void *data)
{
	QVariantMap *newContact = reinterpret_cast<QVariantMap *>(data);
	QVariantMap newContactData = (*newContact)["data"].toMap();

	if (!(((*newContact)["bruteforce"].isNull() == false) && ((*newContact)["bruteforce"].toString() == "true")))
	{
		QVariantMap *datas = new QVariantMap();
		if (this->currentContactId_ == this->src_->count())
			(*datas)["action"] = "new_contact";
		else
		{
			(*this->src_)[this->currentContactId_] = newContactData;
			(*datas)["action"] = "upt_contacts";
		}
		QList<QVariant> list;
		list.append(newContactData);
		(*datas)["data"] = QVariant(list);
		this->eventbus_->emitEvent(JSON_SEND_REQUEST, this, datas);
	}
	else
	{
		if (this->currentContactId_ != this->src_->count())
			(*this->src_)[this->currentContactId_] = newContactData;
	}
	this->contactPanel_ = NULL;
	this->resetContactList();
}

void			MobiiContactPanelForm::contactSelected()
{
	QListWidget *contactList = this->ui.contactContent->findChild<QListWidget*>("listWidget");
	QList<QListWidgetItem *> list = contactList->selectedItems();

	if (list.size() != 0)
	{
		for (int i = 0; i != this->src_->count(); i++)
		{
			QVariantMap contact = (*this->src_)[i].toMap();
			if (!list[0]->text().compare(contact["firstname"].toString() + " " + contact["lastname"].toString()))
			{
				QVariantMap *tmp = &this->src_->at(i).toMap();
				(*tmp)["isModif"] = "true";
				this->buildDataModel(i);
			}
		}
	}
}

void			MobiiContactPanelForm::newContact()
{
	if (this->src_ == NULL)
	{
		Helper::ExecuteInfoMsgBox("Synchroniser avant.");
		return;
	}
	this->currentContactId_ = this->src_->count();
	if (this->contactPanel_ == NULL)
	{
		this->contactPanel_= new MobiiContactInformations(NULL);
		this->contactPanel_->setEventBus(this->eventbus_);
		this->contactPanel_->setStyleSheet(UI_DEFAULT_STYLE);
	}
	else
		this->contactPanel_->setContactData(NULL);
	this->contactPanel_->show();
}

void			MobiiContactPanelForm::buildDataModel(int i)
{
	QVariantMap contact = (*this->src_)[i].toMap();
	this->currentContactId_ = i;
	if (this->contactPanel_ == NULL)
	{
		this->contactPanel_ = new MobiiContactInformations(&contact);
		this->contactPanel_->setEventBus(this->eventbus_);
		this->contactPanel_->setStyleSheet(UI_DEFAULT_STYLE);
	}
	else
		this->contactPanel_->setContactData(&contact);
	this->contactPanel_->show();
}

void			MobiiContactPanelForm::RetrieveContactsClicked()
{
	this->eventbus_->emitEvent(BLOCKING_OPERATION_BEGIN);
	QVariantMap *data = new QVariantMap();
	(*data)["action"] = "req_contacts";
	this->eventbus_->emitEvent(JSON_SEND_REQUEST, this, data);
}

void			MobiiContactPanelForm::SendContactsToMobile()
{
	if (this->src_ == NULL)
	{
		Helper::ExecuteInfoMsgBox("Synchroniser avant.");
		return;
	}
	QVariantMap *data = new QVariantMap();
	(*data)["action"] = "upt_contacts";
	QList<QVariant> list;
	for (int i = 0; i < this->src_->count(); i++)
	{
		QVariantMap tmp = this->src_->at(i).toMap();
		if ((tmp["isModif"].isNull() == false) && (tmp["isModif"].toString() == "true"))
			list.append(this->src_->at(i));
	}

	if (list.count() == 0)
	{
		Helper::ExecuteInfoMsgBox("Aucun contact n'a été modifié.");
		return;
	}
	else
		Helper::ExecuteInfoMsgBox(list.count() + " contacts ont été envoyer au mobile");
	(*data)["data"] = QVariant(list);
	this->eventbus_->emitEvent(JSON_SEND_REQUEST, this, data);
}

void			MobiiContactPanelForm::ContactsReceived(const QString eventName, void *sender, void *data)
{
	this->eventbus_->emitEvent(BLOCKING_OPERATION_END);
	QVariantMap *src = reinterpret_cast<QVariantMap *>(data);
	QListWidget *contactList = this->ui.contactContent->findChild<QListWidget*>("listWidget");

	contactList->clear();
	
	for (int i = 0; i != (*src)["data"].toList().count(); i++)
	{
		QVariantMap data = (*src)["data"].toList()[i].toMap();
		QListWidgetItem *n = new QListWidgetItem(data["firstname"].toString() + " " + data["lastname"].toString());	
		contactList->addItem(n);
	}
	contactList->sortItems();
	if (this->src_ != NULL)
		delete this->src_;
	this->src_ = new QList<QVariant>((*src)["data"].toList());
}

void			MobiiContactPanelForm::OnDisconnected(const QString eventName, void *sender, void *data)
{
	qDebug() << "OnDisconnected MobiiContactPanel ENTRER";
	this->eventbus_->emitEvent(BLOCKING_OPERATION_END);
	if (this->contactPanel_ != NULL)
		this->contactPanel_->close();

	if (this->src_ != NULL)
	{
		for (int i = 0; i != this->src_->count(); i++)
		{
				QVariantMap data = (*this->src_)[i].toMap();
				this->rawContactsDatas_.append(data);
		}
	}
	else
		return;

	// Save contacts table to the device data file
	DeviceEntity *device = this->sessioncon_->currentDevice();
	if (device == NULL)
	{
		qDebug() << "ERROR : Device == NULL";
		return;
	}
	device->writeDatasForModule<MobiiContactPanelForm::TContactsBackupContainer>("contacts", this->rawContactsDatas_);
	this->rawContactsDatas_.clear();

	QListWidget *contactList = this->ui.contactContent->findChild<QListWidget*>("listWidget");
	contactList->clear();

	if (this->src_ != NULL)
	{
		delete this->src_;
		this->src_ = NULL;
	}
	if (this->contactPanel_ != NULL)
		this->contactPanel_ = NULL;
	qDebug() << "OnDisconnected MobiiContactPanel SORTIE";
}

void			MobiiContactPanelForm::OnConnected(const QString eventName, void *sender, void *data)
{
	// Loads SMS stored from previous session
	DeviceEntity *device = this->sessioncon_->currentDevice();
	if (device == NULL)
	{
		qDebug() << "ERROR : Device == NULL";
		return;
	}
	MobiiContactPanelForm::TContactsBackupContainer *backup = device->readDatasForModule<MobiiContactPanelForm::TContactsBackupContainer>("contacts");

	if (backup != NULL) // NULL means that there is no backup yet
	{
		qDebug() << "I found" << backup->count() << " contacts in the backup file!";
		QVariantMap *newContacts = new QVariantMap();
		QList<QVariantMap> *list = backup;
		QList<QVariant> *listC = new QList<QVariant>();
		
		for (int i = 0; i < list->count(); i++)
			listC->append(list->at(i));
		
		(*newContacts)["data"] = QVariant(*listC);
		this->ContactsReceived("", NULL, newContacts);
	}
}

void			MobiiContactPanelForm::fileReceived(const QString eventName, void *eventEmitter, void *data)
{
	QVariantMap *src = reinterpret_cast<QVariantMap *>(data);

	if ((*src)["type"].toString() == "ProfilePic")
	{
		QFile tmp(this->config_.userStorageDir() + "/ProfileImages/" + (*src)["name"].toString());

		tmp.open(QIODevice::ReadWrite | QIODevice::Truncate);
		QByteArray decode64 = QByteArray::fromBase64((*src)["data"].toByteArray());
		qDebug() << tmp.write(decode64.data(), (*src)["size"].toInt());
		
	}
	else
	{
		qDebug() << "MobiiHomeForm, fileReceived: file type not recognized";
		return;
	}
}

void			MobiiContactPanelForm::findNameWithPhone(const QString eventName, void *eventEmitter, void *data)
{
	QMap<QString, QString> *datas = reinterpret_cast<QMap<QString, QString> *>(data);
	QMap<QString, QString>::Iterator it;

	if (this->src_ != NULL)
	{
		for (it = datas->begin(); it != datas->end(); ++it)
		{
			int i = 0;
			for (; i != this->src_->count(); i++)
			{
				QVariantMap contact = (*this->src_)[i].toMap();
				if ((contact["phone1"].toString() == it.key()) | (contact["phone2"].toString() == it.key()))
				{
					it.value() = contact["firstname"].toString() + " " + contact["lastname"].toString();
					break;
				}
			}
		}
		this->eventbus_->emitEvent(CONTACT_NAME_FOR_PHONENB, this, datas);
	}
}

