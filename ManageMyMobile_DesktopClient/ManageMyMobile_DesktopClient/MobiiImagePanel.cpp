#include <qlistwidget.h>
#include <qfiledialog.h>
#include <qstandarditemmodel.h>
#include "MobiiImagePanel.h"
#include "helper.h"

MobiiFilePanel::MobiiFilePanel(QWidget *parent)
	: QWidget(parent)
{
	this->firstTimeInWidget = true;
	ui.setupUi(this);
	QDir phoneImgDir(this->config_.userStorageDir() + "/PhonePic");

	if (phoneImgDir.exists() == false)
		phoneImgDir.mkdir(this->config_.userStorageDir() + "/PhonePic");

	this->currentImg_ = 0;
	this->src_ = NULL;
	//this->src_->clear();

	/*QVariantMap *imageFolder = new QVariantMap();
	QList<QVariant> imageList;
	QVariantMap *img1 = new QVariantMap();
	(*img1)["id"] = "2";
	(*img1)["name"] = "buddy.png";
	(*img1)["date"] = "2013-09-07T10:00:00-05:00";
	QVariantMap *img = new QVariantMap();
	(*img)["id"] = "1";
	(*img)["name"] = "android.png";
	(*img)["date"] = "2013-09-07T11:00:00-05:00";
	imageList.append(*img);
	imageList.append(*img1);
	(*imageFolder)["folderfiles"] = QVariant(imageList);
	(*imageFolder)["foldername"] = "Images";

	QVariantMap *textFolder = new QVariantMap();
	QList<QVariant> textList;
	QVariantMap *txt1 = new QVariantMap();
	(*txt1)["id"] = "3";
	(*txt1)["name"] = "test.txt";
	(*txt1)["date"] = "2013-09-07T10:00:00-05:00";
	textList.append(*txt1);
	(*textFolder)["folderfiles"] = QVariant(textList);
	(*textFolder)["foldername"] = "Fichier Texte";

	QList<QVariant> folderList;
	folderList.append(*textFolder);
	folderList.append(*imageFolder);

	QVariantMap *data = new QVariantMap();
	(*data)["filelist"] = QVariant(folderList);

	this->fileListReceived("", NULL, data);*/

	this->ui.filesTree->setModel(NULL);
	this->ui.filesTree->header()->hide();
	this->ui.currentImg->setText("No file selected");
	//connect(this->ui.requestFiles, SIGNAL(clicked()), this, SLOT(requestFiles()));
	connect(this->ui.deleteFile, SIGNAL(clicked()), this, SLOT(deletefile()));
	connect(this->ui.saveFile, SIGNAL(clicked()), this, SLOT(saveFile()));
}

void	MobiiFilePanel::OnWidgetShow(void)
{
	if (this->firstTimeInWidget == true)
	{
		this->requestFiles();
		this->firstTimeInWidget = false;
	}
}

void	MobiiFilePanel::OnWidgetClose(void)
{

}

MobiiFilePanel::~MobiiFilePanel()
{

}

/* register to events */
void			MobiiFilePanel::OnEventBusSet(void)
{
	this->eventbus_->registerAsConsumer(MobiiEventBusHelper::eventNameFromAction("file"), this, SLOT(fileReceived(const QString, void *, void *)));
	this->eventbus_->registerAsConsumer(MobiiEventBusHelper::eventNameFromAction("filelist"), this, SLOT(fileListReceived(const QString, void *, void *)));
	this->eventbus_->registerAsConsumer(NET_MOBILE_CONNECTED, this, SLOT(OnConnected(const QString, void *, void *)));
	this->eventbus_->registerAsConsumer(NET_MOBILE_DISCONNECTED, this, SLOT(OnDisconnected(const QString, void *, void *)));
}

void			MobiiFilePanel::OnSessionControllerSet(void)
{
}

/* list of file received event */
void			MobiiFilePanel::fileListReceived(const QString eventName, void *eventEmitter, void *data)
{
	this->eventbus_->emitEvent(BLOCKING_OPERATION_END);
	QVariantMap *src = reinterpret_cast<QVariantMap *>(data);
	QStandardItemModel *dataModel = new QStandardItemModel();
	QList<QVariant> folderList = (*src)["filelist"].toList();
	for (int i = 0; i != folderList.count(); i++)
	{
		QVariantMap  data = folderList[i].toMap();
		QStandardItem *parentNode = new QStandardItem(data["foldername"].toString());
		parentNode->setEditable(false);
		QList<QVariant> fileList = data["folderfiles"].toList();
		for (int j = 0; j != fileList.count(); j++)
		{
			QVariantMap  filedata = fileList[j].toMap();
			QStandardItem *item = new QStandardItem(filedata["name"].toString() + " -- " + filedata["date"].toDateTime().toString("dd.MM.yyyy hh:mm"));

			item->setEditable(false);
			item->setData(filedata["id"].toString(), 666);
			parentNode->appendRow(item);
		}
		dataModel->appendRow(parentNode);
	}	
	this->ui.filesTree->setModel(dataModel);
	this->src_ = new QList<QVariant>((*src)["filelist"].toList());
	QItemSelectionModel *selectionModel= this->ui.filesTree->selectionModel();
    connect(selectionModel, SIGNAL(selectionChanged (const QItemSelection &, const QItemSelection &)),
            this, SLOT(fileSelected(const QItemSelection &, const QItemSelection &)));
}

/* one file received */
void			MobiiFilePanel::fileReceived(const QString eventName, void *eventEmitter, void *data)
{
	QVariantMap *src = reinterpret_cast<QVariantMap *>(data);
	if ((*src)["type"].toString() == "PhonePic")
	{
		QFile tmp(this->config_.userStorageDir() + "/PhonePic/" + (*src)["name"].toString());

		tmp.open(QIODevice::ReadWrite | QIODevice::Truncate);
		/* convert from base64 */
		QByteArray decode64 = QByteArray::fromBase64((*src)["data"].toByteArray());
		tmp.write(decode64);
		if (tmp.isOpen() == true)
			tmp.close();
		if (this->ui.filesTree->currentIndex().data(666).toString() == (*src)["id"].toString())
		{
			QImage *tmp = new QImage(this->config_.userStorageDir() + "/PhonePic/" + (*src)["name"].toString());
			QImage resizedImg;
			if (tmp->height() > 480 || tmp->width() > 640)
				resizedImg = tmp->scaled(this->ui.currentImg->maximumHeight(), this->ui.currentImg->maximumWidth(),Qt::KeepAspectRatio);
			else
				resizedImg = *tmp;
			this->ui.currentImg->setPixmap(QPixmap::fromImage(resizedImg));
			delete tmp;
		}
	}
	else if ((*src)["type"].toString() == "Miscellaneous")
	{
		QFile tmp(this->config_.userStorageDir() + "/PhonePic/" + (*src)["name"].toString());
		tmp.open(QIODevice::ReadWrite | QIODevice::Truncate);
		QByteArray decode64 = QByteArray::fromBase64((*src)["data"].toByteArray());
		if (this->ui.filesTree->currentIndex().data(666).toInt() == (*src)["id"].toInt())
		{
			this->ui.currentImg->setText("Pas d'affichage disponible pour ce type de fichier");
		}
		if (tmp.isOpen() == true)
			tmp.close();
	}
	else
	{
		qDebug() << "MobiiFilePanel, fileReceived: file type not recognized";
		return;
	}
}

void			MobiiFilePanel::OnDisconnected(const QString eventName, void *sender, void *data)
{
	qDebug() << "OnDisconnected MobiiImagePanel ENTRER";
	this->eventbus_->emitEvent(BLOCKING_OPERATION_END);
	if (this->src_ != NULL && this->src_->size() != 0)
	{
		for (int i = 0; i != this->src_->size(); i++)
		{
			QVariantMap data = this->src_->at(i).toMap();
			this->rawImagesDatas_.append(data);
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
	device->writeDatasForModule<MobiiFilePanel::TImagesBackupContainer>("images", this->rawImagesDatas_);
	this->rawImagesDatas_.clear();

	this->src_->clear();
	qDebug() << "OnDisconnected MobiiImagePanel SORTIE";
}

void			MobiiFilePanel::OnConnected(const QString eventName, void *sender, void *data)
{
	// Loads SMS stored from previous session
	DeviceEntity *device = this->sessioncon_->currentDevice();
	if (device == NULL)
	{
		qDebug() << "ERROR : Device == NULL";
		return;
	}
	MobiiFilePanel::TImagesBackupContainer *backup = device->readDatasForModule<MobiiFilePanel::TImagesBackupContainer>("images");

	if (backup != NULL && backup->size() != 0) // NULL means that there is no backup yet
	{
		qDebug() << "I found" << backup->count() << " images in the backup file!";
		QVariantMap *imageList = new QVariantMap();
		QList<QVariantMap> *list = backup;
		QList<QVariant> *listC = new QList<QVariant>();
		
		for (int i = 0; i < list->count(); i++)
			listC->append(list->at(i));
		
		(*imageList)["data"] = QVariant(*listC);
		this->fileListReceived("", NULL, imageList);
	}
}

/* reset the image list, after a delete for example */
void			MobiiFilePanel::resetImageList()
{
	QStandardItemModel *dataModel = new QStandardItemModel();
	QList<QVariant> folderList = *this->src_;
	for (int i = 0; i != folderList.count(); i++)
	{
		QVariantMap  data = folderList[i].toMap();
		QStandardItem *parentNode = new QStandardItem(data["foldername"].toString());
		QList<QVariant> fileList = data["folderfiles"].toList();
		for (int j = 0; j != fileList.count(); j++)
		{
			QVariantMap  filedata = fileList[j].toMap();
			QStandardItem *item = new QStandardItem(filedata["name"].toString() + " -- " + filedata["date"].toDateTime().toString("dd.MM.yyyy hh:mm"));

			item->setData(filedata["id"].toString(), 666);
			parentNode->appendRow(item);
		}
		dataModel->appendRow(parentNode);
	}	
	this->ui.filesTree->setModel(dataModel);
	this->ui.nameFile->setText("");
}

void			MobiiFilePanel::deleteFile()
{
	if (this->src_ == NULL)
	{
		Helper::ExecuteInfoMsgBox("Synchroniser avant.");
		return;
	}

	QVariantMap currentImage = (*this->src_)[this->currentImg_].toMap();

	if (this->currentImg_ == 0 && this->src_->size() == 0)
		return;
	QVariantMap *data = new QVariantMap();
	(*data)["action"] = "del_file";
	(*data)["id"] = currentImage["id"].toString();
	(*data)["name"] = currentImage["name"].toString();
	this->src_->removeAt(this->currentImg_);
	this->resetImageList();
	this->currentImg_ = 0;
	if (this->src_->size() == 0)
		return;
	else
		this->ui.filesTree->setCurrentIndex(this->ui.filesTree->rootIndex());

	this->eventbus_->emitEvent(JSON_SEND_REQUEST, this, data);
}

void			MobiiFilePanel::saveFile()
{
	if (this->src_ == NULL)
	{
		Helper::ExecuteInfoMsgBox("Synchroniser avant.");
		return;
	}
	QString dossier = QFileDialog::getExistingDirectory(this, "Où copier le fichier ?");

	if (dossier.size() != 0)
	{
		QString id = this->ui.filesTree->currentIndex().data(666).toString();
		for (int i = 0; i < this->src_->count(); i++)
		{
			QVariantMap currentFolder = (*this->src_)[i].toMap();
			QList<QVariant> folderList = currentFolder["folderfiles"].toList();
			for (int j = 0; j < folderList.count(); j++)
			{
				QVariantMap  filedata = folderList[j].toMap();
				if (id == filedata["id"].toString())
				{
					QFile tmp(this->config_.userStorageDir() + "/PhonePic/" + filedata["name"].toString());
					tmp.open(QIODevice::ReadOnly);
					if (!tmp.copy(dossier + "/" + filedata["name"].toString()) == true)
						qDebug() << "Copy file failed";
				}
			}
		}

	}
}

void			MobiiFilePanel::requestFiles()
{
	this->eventbus_->emitEvent(BLOCKING_OPERATION_BEGIN);
	QVariantMap *data = new QVariantMap();
	(*data)["action"] = "req_files";
	this->eventbus_->emitEvent(JSON_SEND_REQUEST, this, data);
}

void			MobiiFilePanel::fileSelected(const QItemSelection &file1, const QItemSelection &file2)
{
	this->ui.currentImg->clear();
	QString id = this->ui.filesTree->currentIndex().data(666).toString();
	for (int i = 0; i < this->src_->count(); i++)
	{
		QVariantMap currentFolder = (*this->src_)[i].toMap();
		QList<QVariant> folderList = currentFolder["folderfiles"].toList();
		for (int j = 0; j < folderList.count(); j++)
		{
			QVariantMap  filedata = folderList[j].toMap();
			if (id == filedata["id"].toString())
			{
				QFile tmp(this->config_.userStorageDir() + "/PhonePic/" + filedata["name"].toString());
				if (tmp.exists() == true)
				{
					QImage *tmp = new QImage(this->config_.userStorageDir() + "/PhonePic/" +  filedata["name"].toString());
					QImage resizedImg;
					if (tmp->height() > 480 || tmp->width() > 640)
						resizedImg = tmp->scaled(this->ui.currentImg->maximumHeight(), this->ui.currentImg->maximumWidth(),Qt::KeepAspectRatio);
					else
						resizedImg = *tmp;
					if (tmp->isNull() == true)
						this->ui.currentImg->setText("Pas d'affichage disponible pour ce type de fichier.");
					else
						this->ui.currentImg->setPixmap(QPixmap::fromImage(resizedImg));
					this->ui.nameFile->setText(filedata["name"].toString());
					delete tmp;
				}
				else
				{
					QVariantMap *data = new QVariantMap();
					(*data)["action"] = "req_this_image";
					(*data)["id"] =  filedata["id"].toString();
					this->eventbus_->emitEvent(JSON_SEND_REQUEST, this, data);
				}
				if (tmp.isOpen() == true)
					tmp.close();
				break;
			}
		}
	}
}