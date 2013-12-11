/*
** Mobii Project -- version 0.0.1000 DEVELOPMENT VERSION
**
** Copyright(C) 2012 Mobii contributors.
**
** This library is provided `as is' and without any warranty.
** Authors decline any responsibility for any damage directly or
** indirectly caused by the use of this library.
*/

#include <sstream>
#include <qwidget.h>
#include <qtablewidget.h>
#include <qheaderview.h>
#include <qmessagebox.h>
#include <qcryptographichash.h>

#include "commons.h"
#include "UserEntity.h"
#include "MobiiEventBusCodes.h"
#include "MobiiConnectToMobileForm.h"
#include "ConfigurationFileIo.h"


MobiiConnectToMobileForm::MobiiConnectToMobileForm(void)
{
	this->ui.setupUi(this);

	this->m_delKeyDetector = new QShortcut(QKeySequence(QKeySequence::Delete), this->ui.alreadyConnectedMobileList);

	connect(this->ui.AttemptBindingBtn, SIGNAL(clicked()), this, SLOT(Slot_ExecuteConnectionForNewPhone()));
	connect(this->ui.alreadyConnectedMobileList, SIGNAL(cellChanged(int, int)), this, SLOT(Slot_cellHasChanged(int, int)));
	connect(this->ui.alreadyConnectedMobileList, SIGNAL(cellClicked(int, int)), this, SLOT(Slot_selectionChanged(int, int)));
	connect(this->m_delKeyDetector, SIGNAL(activated()), this, SLOT(Slot_deleteSelectedEntry()));
	connect(this->ui.deleteRowButton, SIGNAL(clicked()), this, SLOT(Slot_deleteSelectedEntry()));
	connect(this->ui.alreadyConnectedMobileList, SIGNAL(cellDoubleClicked(int, int)), this, SLOT(Slot_cellDoubleClicked(int, int)));
}


MobiiConnectToMobileForm::~MobiiConnectToMobileForm(void)
{
}


void		MobiiConnectToMobileForm::OnEventBusSet(void)
{
	this->eventbus_->registerAsConsumer(NET_MOBILE_CONNECTED, this, SLOT(Slot_mobileConnected(const QString, void *, void *)));
	this->eventbus_->registerAsConsumer(NET_SERVER_DISCONNECTED, this, SLOT(Slot_userLoggedOut(const QString, void *, void *)));
	this->eventbus_->registerAsConsumer(USER_LOGIN_OKAY, this, SLOT(Slot_userLoggedIn(const QString, void *, void *)));
	this->eventbus_->registerAsConsumer(USER_MOBILES_LIST, this, SLOT(Slot_parseDevicesListFromJson(const QString, void *, void *)));
	this->eventbus_->registerAsConsumer(MobiiEventBusHelper::eventNameFromAction("mobile_infos"), this, SLOT(Slot_mobileInfosReceived(const QString, void *, void *)));
}


void		MobiiConnectToMobileForm::OnSessionControllerSet(void)
{
	this->loadMobileInfosFromDisk();
}


void		MobiiConnectToMobileForm::updateMobilesListOnUi(void)
{
	/// Clears the GUI array
	QTableWidget *guiTable = this->ui.alreadyConnectedMobileList;
	guiTable->clearContents();
	guiTable->setRowCount(0);

	/// Then refill the GUI
	for (TMobileInfosList::iterator it = this->m_mobilesList.begin(); it != this->m_mobilesList.end(); ++it)
	{
		guiTable->insertRow(guiTable->rowCount());
		int currentline = guiTable->rowCount() - 1;
		guiTable->setItem(currentline, 0, this->makeTableCell((*it)["name"], true));
		guiTable->setItem(currentline, 1, this->makeTableCell((*it)["imei"]));
		guiTable->setItem(currentline, 2, this->makeTableCell((*it)["model"]));
		guiTable->setItem(currentline, 3, this->makeTableCell((*it)["manufacturer"]));
		guiTable->setItem(currentline, 4, this->makeTableCell(it.key()));
	}
	// Not working as intended
	//guiTable->resizeColumnsToContents();
}


int			MobiiConnectToMobileForm::getSelectedRow(void)
{
	QList<QTableWidgetSelectionRange> ranges = this->ui.alreadyConnectedMobileList->selectedRanges();
	if (ranges.size() != 1)
		return -1; // No cell or row selected
	QTableWidgetSelectionRange selectionRange = ranges.front();
	if (selectionRange.topRow() != selectionRange.bottomRow())
		return -1; // Several rows selected, invalid
	return selectionRange.topRow();
}


void		MobiiConnectToMobileForm::Slot_cellHasChanged(int row, int col)
{
	if (this->getSelectedRow() == row)
	{
		QTableWidgetItem *namecell = this->ui.alreadyConnectedMobileList->item(row, col);
		QTableWidgetItem *uidcell = this->ui.alreadyConnectedMobileList->item(row, UID_COLUMN_IN_HISTORY_ARRAY);
		if (this->m_mobilesList.find(uidcell->text()) == this->m_mobilesList.end())
		{
			qDebug() << "[CAUTION] No such entry in mobiles history:" << uidcell->text();
			return;
		}
		this->m_mobilesList[uidcell->text()]["name"] = namecell->text();
		this->saveRenameMobile(this->m_mobilesList[uidcell->text()]);
	}
}


void		MobiiConnectToMobileForm::Slot_cellDoubleClicked(int row, int col)
{
	if (col != 0) // If user double clicks on the name cell, do not launch connection
		this->Slot_ExecuteConnectionForNewPhone();
}


void		MobiiConnectToMobileForm::Slot_deleteSelectedEntry(void)
{
	int row = this->getSelectedRow();
	if (row == -1)
		return;

	// Locate the requested entry in the internal model.
	QString uidToDelete = this->ui.alreadyConnectedMobileList->item(row, UID_COLUMN_IN_HISTORY_ARRAY)->text();
	if (this->m_mobilesList.find(uidToDelete) == this->m_mobilesList.end())
	{
		qDebug() << "[CAUTION] Mobile with UID" << uidToDelete << "has no counterpart in the internal model";
		return;
	}
	this->saveDeletedMobile(this->m_mobilesList[uidToDelete]);
	this->updateMobilesListOnUi();
}


void		MobiiConnectToMobileForm::Slot_selectionChanged(int row, int col)
{
	QTableWidgetItem *cell = this->ui.alreadyConnectedMobileList->item(row, UID_COLUMN_IN_HISTORY_ARRAY);
	this->ui.ConnectionCodeField->setText(cell->text());
	
	ConfigurationFileIo cfg;
	QString passwordField;
	if (cfg.phonePasswordsAutosave() && this->m_mobilesList[cell->text()].contains("password"))
		passwordField = this->m_mobilesList[cell->text()]["password"];
	this->ui.NewMobilePasswordField->setText(passwordField);
}


void		MobiiConnectToMobileForm::Slot_userLoggedIn(const QString eventName, void *sender, void *data)
{
	// Clear existing entries
	this->m_mobilesList.clear();
	this->updateMobilesListOnUi();

	// Logged user >> request the list to server
	QVariantMap *json = new QVariantMap();
	(*json)["action"] = "list";
	this->eventbus_->emitEvent(USER_MOBILES_SENDREQ, this, json);
}


void		MobiiConnectToMobileForm::Slot_userLoggedOut(const QString eventName, void *sender, void *data)
{
	// Clear everything
	this->ui.alreadyConnectedMobileList->clearContents();
	this->ui.alreadyConnectedMobileList->setRowCount(0);
	this->m_mobilesList.clear();

	// By default, refill with the local devices list
	this->loadMobileInfosFromDisk();
}


void		MobiiConnectToMobileForm::Slot_parseDevicesListFromJson(const QString eventName, void *sender, void *data)
{
	QVariantMap *json = (QVariantMap *)data;

	// Parse each item
	QVariantList mobileList = (*json)["favoris"].toList();
	for (QVariantList::iterator it = mobileList.begin(); it != mobileList.end(); ++it)
	{
		// Failsafe
		QVariantMap mobileEntries = it->toMap();
		if (mobileEntries.find("uid") == mobileEntries.end())
		{
			qDebug() << Q_FUNC_INFO << "Server has sent invalid mobile informations for this user.";
			continue;
		}
		
		TMobileInfos mobileInfos;
		for (QVariantMap::iterator entry = mobileEntries.begin(); entry != mobileEntries.end(); ++entry)
			mobileInfos[entry.key()] = entry->toString();
		this->m_mobilesList[mobileInfos["uid"] ] = mobileInfos;
	}

	// Refresh all that
	this->updateMobilesListOnUi();
}


void		MobiiConnectToMobileForm::Slot_mobileConnected(const QString eventName, void *sender, void *data)
{
	QByteArray rawDeviceUid = this->sessioncon_->currentDevice()->id();
	QString connectedDeviceUid = QString(Commons::hexToLitteralString(rawDeviceUid.data(), rawDeviceUid.size(), "").c_str()).toUpper();
	
	// Phone not in list yet, to be added
	TMobileInfosList::iterator mobileIt = this->m_mobilesList.find(connectedDeviceUid);
	if (mobileIt == this->m_mobilesList.end())
	{
		MobiiConnectToMobileForm::TMobileInfos infos;
		infos["uid"] = connectedDeviceUid;
		infos["name"] = "<sans nom>";
		this->m_mobilesList[connectedDeviceUid] = infos;
		this->saveNewMobile(infos);
	}

	// Anyway, save the phone password if permitted
	ConfigurationFileIo cfg;
	if (cfg.phonePasswordsAutosave())
	{
		TMobileInfosList::iterator mobileIt = this->m_mobilesList.find(connectedDeviceUid);
		(*mobileIt)["password"] = this->ui.NewMobilePasswordField->text();
		this->saveNewMobile(*mobileIt);
	}

	// Clear UI fields
	this->ui.ConnectionCodeField->clear();
	this->ui.NewMobilePasswordField->clear();

	this->updateMobilesListOnUi();
}


QTableWidgetItem *MobiiConnectToMobileForm::makeTableCell(const QString &content, bool editable)
{
	QTableWidgetItem *item = new QTableWidgetItem(content);
	item->setFlags(Qt::NoItemFlags | Qt::ItemIsSelectable | Qt::ItemIsEnabled);
	if (editable == true)
		item->setFlags(item->flags() | Qt::ItemIsEditable);
	return item;
}


void		MobiiConnectToMobileForm::Slot_mobileInfosReceived(const QString eventName, void *sender, void *data)
{
	// Failsafe
	if (this->sessioncon_->currentDevice() == NULL)
		return;

	QByteArray rawDeviceUid = this->sessioncon_->currentDevice()->id();
	QString connectedDeviceUid = QString(Commons::hexToLitteralString(rawDeviceUid.data(), rawDeviceUid.size(), "").c_str()).toUpper();

	// Updates the internal storage model then applies on GUI and save
	QVariantMap src = (*reinterpret_cast<QVariantMap *>(data))["args"].toMap();
	for (QVariantMap::iterator it = src.begin(); it != src.end(); ++it)
		this->m_mobilesList[connectedDeviceUid][it.key()] = (*it).toString();
	this->updateMobilesListOnUi();
	this->saveMobileInfosOnDisk();
}


void		MobiiConnectToMobileForm::Slot_ExecuteConnectionForNewPhone(void)
{
	QString temp;

	// Convert the string into the right transition format
	temp.append(this->ui.ConnectionCodeField->text().toUpper());
	temp.remove(' ');
	if (temp.length() != 12)
	{
		QMessageBox box(QMessageBox::Critical, "Code de connexion invalide", "Le code que vous avez entré n'est pas valide.\nVérifiez votre saisie puis réessayez.");
		box.exec();
		return;
	}

	// Convert to byte array
	QByteArray uidBytes;
	for (int i = 0; i != temp.size(); i += 2)
	{
		unsigned int val;
		std::string substr = temp.mid(i, 2).toStdString();
		std::stringstream ss(substr);
		ss >> std::hex >> val;
		uidBytes.append((unsigned char)val);
	}

	// Store the converted UID and password to a QPair
	QPair<QByteArray, QString> *data = new QPair<QByteArray, QString>();
	(*data).first = uidBytes;
	(*data).second = this->ui.NewMobilePasswordField->text();

	// Sends the packet and notifies whoever is concerned that an attempt has been made
	this->eventbus_->emitEvent(ATTEMPT_MOBILE_BINDING, this, data);
}


void		MobiiConnectToMobileForm::saveMobileInfosOnDisk(void)
{
	UserEntity *currUser = this->sessioncon_->currentUser();
	if (currUser == NULL)
		return;
	currUser->writeDatasForModule<MobiiConnectToMobileForm::TMobileInfosList>("history", this->m_mobilesList);
}


void		MobiiConnectToMobileForm::loadMobileInfosFromDisk(void)
{
	UserEntity *currUser = this->sessioncon_->currentUser();
	if (currUser == NULL)
		return;
	MobiiConnectToMobileForm::TMobileInfosList *tmp = currUser->readDatasForModule<MobiiConnectToMobileForm::TMobileInfosList>("history");
	if (tmp != NULL)
	{
		this->m_mobilesList = *tmp;
		delete tmp;
	}
	this->updateMobilesListOnUi();
}


void		MobiiConnectToMobileForm::saveNewMobile(const TMobileInfos &mobile)
{
	if (this->isUserLoggedIn())
	{
		QVariantMap item;
		item["uid"] = mobile["uid"];
		item["name"] = mobile["name"];

		QVariantList list;
		list.push_back(item);

		QVariantMap *json = new QVariantMap();
		(*json)["action"] = "add";
		(*json)["favoris"] = list;
		this->eventbus_->emitEvent(USER_MOBILES_SENDREQ, this, json);
	}
	else // Save to disk
		this->saveMobileInfosOnDisk();
}


void		MobiiConnectToMobileForm::saveDeletedMobile(const MobiiConnectToMobileForm::TMobileInfos &mobile)
{
	MobiiConnectToMobileForm::TMobileInfos backup = mobile;
	this->m_mobilesList.remove(backup["uid"]);
	if (this->isUserLoggedIn())
	{
		QVariantMap item;
		item["uid"] = backup["uid"];

		QVariantList list;
		list.push_back(item);

		QVariantMap *json = new QVariantMap();
		(*json)["action"] = "remove";
		(*json)["favoris"] = list;
		this->eventbus_->emitEvent(USER_MOBILES_SENDREQ, this, json);
	}
	else
		this->saveMobileInfosOnDisk();
}


void		MobiiConnectToMobileForm::saveRenameMobile(const MobiiConnectToMobileForm::TMobileInfos &mobile)
{
	if (this->isUserLoggedIn())
	{
		QVariantMap item;
		item["uid"] = mobile["uid"];
		item["name"] = mobile["name"];

		QVariantList list;
		list.push_back(item);

		QVariantMap *json = new QVariantMap();
		(*json)["action"] = "rename";
		(*json)["favoris"] = list;
		this->eventbus_->emitEvent(USER_MOBILES_SENDREQ, this, json);
	}
	else
		this->saveMobileInfosOnDisk();
}


bool		MobiiConnectToMobileForm::isUserLoggedIn(void)
{
	return this->sessioncon_->currentUser()->username() != USERENTITY_DEFAULT;
}