#include <qmessagebox.h>
#include <qtablewidget.h>
#include <qdebug.h>
#include <qvariant.h>

#include "MobiiEventBusCodes.h"
#include "MobiiPhoneConnectionControlForm.h"


MobiiPhoneConnectionControlForm::MobiiPhoneConnectionControlForm(void)
{
	this->ui.setupUi(this);
	
	this->initializeMatrix();
	connect(this->ui.cleanAllDatasBtn, SIGNAL(clicked()), this, SLOT(OnPurgeAllDatasClicked()));
}


MobiiPhoneConnectionControlForm::~MobiiPhoneConnectionControlForm(void)
{
}


void		MobiiPhoneConnectionControlForm::OnEventBusSet(void)
{
	this->eventbus_->registerAsConsumer(MobiiEventBusHelper::eventNameFromAction("mobile_infos"), this, SLOT(do_processMobileInfos(const QString, void *, void *)));
	this->eventbus_->registerAsConsumer(NET_MOBILE_DISCONNECTED, this, SLOT(OnDisconnected(const QString, void *, void *)));
}


void		MobiiPhoneConnectionControlForm::OnDisconnected(const QString, void *, void *)
{
	this->ui.MobileInfosTable->clearContents();
	this->ui.MobileInfosTable->setRowCount(0);
}


void		MobiiPhoneConnectionControlForm::OnPurgeAllDatasClicked(void)
{
	QMessageBox msg(QMessageBox::Warning, "Nettoyer les données locales", "Toutes les données locales qui ont été synchronisées depuis le téléphone vers cet ordinateur seront définitivement supprimées.\nAvant de commencer la suppression, vous allez être déconnecté du téléphone.\n\nSouhaitez-vous vraiment supprimer ces données ?", QMessageBox::Yes | QMessageBox::No);
	int res = msg.exec();
	if (res == QMessageBox::Yes)
	{
		DeviceEntity *device = this->sessioncon_->currentDevice();
		if (device == NULL)
		{
			qDebug() << Q_FUNC_INFO << "No device loaded";
			return;
		}
		qDebug() << "Clearing all local datas for this phone";
		this->eventbus_->emitEvent(DISCONNECT_FROM_SERVER, this, NULL);
		this->eventbus_->emitEvent(BLOCKING_OPERATION_BEGIN, this, NULL);
		if (device->purgeLocalDatas() == false)
		{
			QMessageBox msg(
				QMessageBox::Warning, 
				"Nettoyage incomplet", 
				QString("Certaines données de synchronisation n'ont pas pu être effacées.\nVous pouvez tenter d'effacer manuellement ces données en accédant à ce dossier :\n") + device->locationOnFilesystem());
			msg.exec();
		}
		else
		{
			QMessageBox msg(
				QMessageBox::Information, 
				"Nettoyage terminé", 
				"Toutes les données locales de ce téléphone ont été effacées.");
			msg.exec();
		}
		this->eventbus_->emitEvent(BLOCKING_OPERATION_END, this, NULL);
	}
}


void		MobiiPhoneConnectionControlForm::initializeMatrix(void)
{
	this->nameMatrix_["imei"] = "IMEI";
	this->nameMatrix_["manufacturer"] = "Constructeur";
	this->nameMatrix_["model"] = "Modèle";
	this->nameMatrix_["os"] = "Système d'exploitation";
}


const QString &MobiiPhoneConnectionControlForm::commonNameByKey(const QString &key)
{
	if (this->nameMatrix_.contains(key))
		return this->nameMatrix_[key];
	return key;
}


void		MobiiPhoneConnectionControlForm::do_processMobileInfos(
	const QString eventName,
	void *sender, 
	void *data)
{
	// Parse mobile informations
	QVariantMap src = (*reinterpret_cast<QVariantMap *>(data))["args"].toMap();
	int i = 0;
	for (QVariantMap::iterator it = src.begin(); it != src.end(); ++it)
	{
		this->ui.MobileInfosTable->insertRow(this->ui.MobileInfosTable->rowCount());
		this->ui.MobileInfosTable->setItem(i, 0, new QTableWidgetItem(this->commonNameByKey(it.key())));
		this->ui.MobileInfosTable->setItem(i, 1, new QTableWidgetItem(it.value().toString()));
		++i;
	}
	if (i == 0) // No datas available
	{
		this->ui.MobileInfosTable->insertRow(this->ui.MobileInfosTable->rowCount());
		this->ui.MobileInfosTable->setItem(i, 0, new QTableWidgetItem("Aucune information n'est disponible pour ce téléphone."));
	}

	// Retrieve features which are available on the cellphone
	QString ftlistUi;
	QVector<QString> featuresAvailable;
	QVariantList features = (*reinterpret_cast<QVariantMap *>(data))["features"].toList();
	for (QVariantList::iterator it = features.begin(); it != features.end(); ++it)
	{
		featuresAvailable.push_back(it->toString());
		if (!ftlistUi.isEmpty())
			ftlistUi.append(", ");
		ftlistUi.append(it->toString());
	}
	if (!featuresAvailable.isEmpty())
	{
		this->ui.MobileInfosTable->insertRow(this->ui.MobileInfosTable->rowCount());
		this->ui.MobileInfosTable->setItem(i, 0, new QTableWidgetItem("Fonctions disponibles"));
		this->ui.MobileInfosTable->setItem(i, 1, new QTableWidgetItem(ftlistUi));
	}
	this->eventbus_->emitEvent(FEATURES_AVAILABLE_ON_PHONE, this, &featuresAvailable);

	// UI refresh
	this->ui.MobileInfosTable->resizeColumnsToContents();
}