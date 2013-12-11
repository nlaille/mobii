#include "MobiiContactInformations.h"
#include "helper.h"

MobiiContactInformations::MobiiContactInformations(QVariantMap *contactData) 
{
	this->setModal(false);
	this->ui.setupUi(this);

	connect(this->ui.phoneList, SIGNAL(itemPressed(QListWidgetItem *)), this, SLOT(itemPressed(QListWidgetItem *)));
	connect(this->ui.phoneList, SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)), this, SLOT(checkIfLastPhoneLine(QListWidgetItem*,QListWidgetItem*)));
	connect(this->ui.mailList, SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)), this, SLOT(checkIfLastMailLine(QListWidgetItem*,QListWidgetItem*)));
	connect(this->ui.deleteContactButton, SIGNAL(clicked()), this, SLOT(deleteCurrentContact()));
	connect(this->ui.updateContactButton, SIGNAL(clicked()), this, SLOT(updateCurrentContact()));

	this->setContactData(contactData);
}



void MobiiContactInformations::setContactData(QVariantMap *contactData)
{
	if (contactData != NULL)
	{
		if ((*contactData)["profilePic"].toString().size() != 0)
		{
			QImage *tmp = new QImage(this->config_.userStorageDir() + "/PhoneImages/" + (*contactData)["profilePic"].toString());
			this->ui.pictureLabel->setPixmap(QPixmap::fromImage(*tmp));
		}
		else
		{
			QImage tmp(":/common/Resources/default_profile.jpg");
			tmp.scaled(130, 140, Qt::IgnoreAspectRatio);

			QImage *img = new QImage(130, 140, QImage::Format_RGB32);
			img->loadFromData(tmp.bits(), tmp.byteCount(), "jpg");
			this->ui.pictureLabel->setPixmap(QPixmap::fromImage(*img));
		}

		this->currentID_ = (*contactData)["id"].toString();
		this->ui.firstNameEdit->setText((*contactData)["firstname"].toString());
		this->ui.lastNameEdit->setText((*contactData)["lastname"].toString());
		this->ui.notesTextEdit->setText((*contactData)["notes"].toString());
		if ((*contactData)["birthday"].toString().size() != 0)
			this->ui.birthdayEdit->setDate(QDate::fromString((*contactData)["birthday"].toString(), Qt::ISODate));
		else
			this->ui.birthdayEdit->setDate(QDate::fromString("01/01/1900", "dd/MM/yyyy"));
		this->ui.societyEdit->setText((*contactData)["society"].toString());

		QList<QVariant> phonelist = (*contactData)["phoneList"].toList();
		QList<QVariant> mailList = (*contactData)["mailList"].toList();

		for (int i = 0; i < phonelist.size(); i++)
		{
			QVariantMap  data = phonelist[i].toMap();
			this->addPhoneLine(data["label"].toString(), data["value"].toString());
		}
		/* add an empty one for next entry */
		this->addNewPhoneLine();

		for (int j = 0; j < mailList.size(); j++)
		{
			QVariantMap  data = mailList[j].toMap();
			this->addMailLine(data["label"].toString(), data["value"].toString());
		}
		/* add an empty one for next entry */
		this->addNewMailLine();
	}
	else
	{
		this->ui.birthdayEdit->setDate(QDate::fromString("01/01/1900", "dd/MM/yyyy"));
		QImage tmp(":/common/Resources/default_profile.jpg");
		tmp.scaled(130, 140, Qt::IgnoreAspectRatio);
		this->ui.pictureLabel->setPixmap(QPixmap::fromImage(tmp));

		this->currentID_ = "0";
		this->ui.firstNameEdit->setText("");
		this->ui.lastNameEdit->setText("");
		this->ui.notesTextEdit->setText("");
		this->ui.societyEdit->setText("");
		this->ui.phoneList->clear();
		this->ui.mailList->clear();
		this->addNewMailLine();
		this->addNewPhoneLine();
	}
}

MobiiContactInformations::~MobiiContactInformations()
{
}

void	MobiiContactInformations::updateCurrentContact()
{
	QVariantMap *datas = new QVariantMap();
	QVariantMap *data = new QVariantMap();

	(*data)["isModif"] = "false";
 	(*data)["id"] = this->currentID_;
	(*data)["firstname"] = this->ui.firstNameEdit->text();
	(*data)["lastname"] = this->ui.lastNameEdit->text();
	(*data)["society"] = this->ui.societyEdit->text();

	/* GET PHONE LIST */
	QList<QVariant>* pL = new QList<QVariant>();
	for (int i = 0; i < this->ui.phoneList->count(); i++)
	{
		QWidget* widget = this->ui.phoneList->itemWidget(this->ui.phoneList->item(i)); 
		QVariantMap *data = new QVariantMap();
		QLineEdit *tmp = (QLineEdit *)widget->layout()->itemAt(1)->widget();
		QLabel *tmp2 = (QLabel *)widget->layout()->itemAt(0)->widget();

		(*data)["title"] = tmp2->text();
		(*data)["name"] = tmp->text();
		pL->append(*data);
	}
	(*data)["phoneList"] = QVariant(*pL);

	/* GET MAIL LIST */ 
	QList<QVariant>* mL = new QList<QVariant>();
	for (int i = 0; i < this->ui.mailList->count(); i++)
	{
		QWidget* widget = this->ui.mailList->itemWidget(this->ui.mailList->item(i)); 
		QVariantMap *data = new QVariantMap();
		QLineEdit *tmp = (QLineEdit *)widget->layout()->itemAt(1)->widget();
		QLabel *tmp2 = (QLabel *)widget->layout()->itemAt(0)->widget();

		(*data)["title"] = tmp2->text();
		(*data)["name"] = tmp->text();
		mL->append(*data);
	}
	(*data)["mailList"] = QVariant(*mL);

	(*data)["notes"] = this->ui.notesTextEdit->toPlainText();

	if (this->ui.birthdayEdit->date().toString("dd/MM/yyyy").compare("01/01/1900") < 0)
		(*data)["birthday"] = "";
	else
		(*data)["birthday"] = Helper::ToStringISODateWithTimeZone(this->ui.birthdayEdit->dateTime());

	(*datas)["data"] = QVariant(*data);
	this->eventbus_->emitEvent("update_currentContact", this, datas);
	this->close();
}

/* when the qdialog is closed, update and send to the MobiiContactPanelForm the new datas */
void	MobiiContactInformations::closeEvent(QCloseEvent *evt)
{
	QVariantMap *datas = new QVariantMap();
	QVariantMap *data = new QVariantMap();

	(*data)["isModif"] = "true";
 	(*data)["id"] = this->currentID_;
	(*data)["firstname"] = this->ui.firstNameEdit->text();
	(*data)["lastname"] = this->ui.lastNameEdit->text();
	(*data)["society"] = this->ui.societyEdit->text();

	/* GET PHONE LIST */
	QList<QVariant>* pL = new QList<QVariant>();
	for (int i = 0; i < this->ui.phoneList->count(); i++)
	{
		QWidget* widget = this->ui.phoneList->itemWidget(this->ui.phoneList->item(i)); 
		QVariantMap *data = new QVariantMap();
		QLineEdit *tmp = (QLineEdit *)widget->layout()->itemAt(1)->widget();
		QLabel *tmp2 = (QLabel *)widget->layout()->itemAt(0)->widget();

		(*data)["title"] = tmp2->text();
		(*data)["name"] = tmp->text();
		pL->append(*data);
	}
	(*data)["phoneList"] = QVariant(*pL);

	/* GET MAIL LIST */ 
	QList<QVariant>* mL = new QList<QVariant>();
	for (int i = 0; i < this->ui.mailList->count(); i++)
	{
		QWidget* widget = this->ui.mailList->itemWidget(this->ui.mailList->item(i)); 
		QVariantMap *data = new QVariantMap();
		QLineEdit *tmp = (QLineEdit *)widget->layout()->itemAt(1)->widget();
		QLabel *tmp2 = (QLabel *)widget->layout()->itemAt(0)->widget();

		(*data)["label"] = tmp2->text();
		(*data)["value"] = tmp->text();
		mL->append(*data);
	}
	(*data)["phoneList"] = QVariant(*mL);

	(*data)["notes"] = this->ui.notesTextEdit->toPlainText();

	if (this->ui.birthdayEdit->date().toString("dd/MM/yyyy") == "01/01/1900")
		(*data)["birthday"] = "";
	else
		(*data)["birthday"] = Helper::ToStringISODateWithTimeZone(this->ui.birthdayEdit->dateTime());

	(*datas)["data"] = QVariant(*data);
	(*datas)["bruteforce"] = "true";
	this->eventbus_->emitEvent("update_currentContact", this, datas);
	
	evt->accept();
}

void	MobiiContactInformations::deleteCurrentContact()
{
	QVariantMap *datas = new QVariantMap();

	this->eventbus_->emitEvent("delete_currentContact", this, datas);
	this->close();
}

/// Specifies actions to perform when the event bus is set.
void		MobiiContactInformations::OnEventBusSet(void)
{
}
/// Specifies actions to perform when the session controller is set.
void		MobiiContactInformations::OnSessionControllerSet(void)
{
}

/* redirect on the sms fonction */
void	MobiiContactInformations::startWritingSms()
{
	if (this->ui.phoneList->currentItem() == NULL)
	{
		Helper::ExecuteInfoMsgBox("Pas de ligne sélectionnée.");
		return;
	}

	QWidget* widget = this->ui.phoneList->itemWidget(this->ui.phoneList->currentItem());
	QLineEdit *edit = (QLineEdit *)widget->layout()->itemAt(1)->widget();

	MobiiNewSmsDialog *tmp = new MobiiNewSmsDialog(this->eventbus_, edit->text());
	tmp->exec();
	delete tmp;
}

void	MobiiContactInformations::addNewPhoneLine()
{
	QListWidgetItem *phone = new QListWidgetItem();
	QLabel *lab = new QLabel("Autre");
	QLineEdit* t = new QLineEdit("");
	t->setMinimumWidth(120);
	QPushButton *b = new QPushButton("x");
	b->setMaximumWidth(30);
	QPushButton *p = new QPushButton(QIcon(":/buttons/Resources/write.png"), "");
	p->setMaximumWidth(30);
	QHBoxLayout *l = new QHBoxLayout();
	QWidget *w = new QWidget();
	l->addWidget(lab);
	l->addWidget(t);		
	l->addWidget(b);
	l->addWidget(p);

	w->setLayout(l);
	phone->setSizeHint(w->sizeHint());
	this->ui.phoneList->addItem(phone);
	this->ui.phoneList->setItemWidget(phone, w);

	connect(p, SIGNAL(clicked()), this, SLOT(startWritingSms()));
	connect(b, SIGNAL(clicked()), this, SLOT(removeCurrentPhoneLine()));
}

void	MobiiContactInformations::addNewMailLine()
{
	QListWidgetItem *mail = new QListWidgetItem();
	QLabel *lab = new QLabel("Autre");
	QLineEdit* t = new QLineEdit("");
	t->setMinimumWidth(120);
	QPushButton *b = new QPushButton("x");
	b->setMaximumWidth(30);
	QHBoxLayout *l = new QHBoxLayout();
	QWidget *w = new QWidget();
	l->addWidget(lab);
	l->addWidget(t);		
	l->addWidget(b);

	w->setLayout(l);
	mail->setSizeHint(w->sizeHint());
	this->ui.mailList->addItem(mail);
	this->ui.mailList->setItemWidget(mail, w);

	connect(b, SIGNAL(clicked()), this, SLOT(removeCurrentMailLine()));
}

void	MobiiContactInformations::removeCurrentPhoneLine()
{
	if (this->ui.phoneList->currentItem() == NULL)
	{
		Helper::ExecuteInfoMsgBox("Pas de ligne sélectionnée.");
		return;
	}
	this->ui.phoneList->removeItemWidget(this->ui.phoneList->currentItem());
	delete this->ui.phoneList->currentItem();
}

void	MobiiContactInformations::removeCurrentMailLine()
{
	if (this->ui.mailList->currentItem() == NULL)
	{
		Helper::ExecuteInfoMsgBox("Pas de ligne sélectionnée.");
		return;
	}
	this->ui.mailList->removeItemWidget(this->ui.mailList->currentItem());
	delete this->ui.mailList->currentItem();
}

void	MobiiContactInformations::addPhoneLine(QString title, QString name)
{
	QListWidgetItem *phone = new QListWidgetItem();
	QLabel *lab = new QLabel(title);
	QLineEdit* t = new QLineEdit(name);
	t->setMinimumWidth(120);
	QPushButton *b = new QPushButton("x");
	b->setMaximumWidth(30);
	QPushButton *p = new QPushButton(QIcon(":/buttons/Resources/write.png"), "");
	p->setMaximumWidth(30);
	QHBoxLayout *l = new QHBoxLayout();
	QWidget *w = new QWidget();
	l->addWidget(lab);
	l->addWidget(t);		
	l->addWidget(b);
	l->addWidget(p);

	w->setLayout(l);
	phone->setSizeHint(w->sizeHint());
	this->ui.phoneList->addItem(phone);
	this->ui.phoneList->setItemWidget(phone, w);

	connect(p, SIGNAL(clicked()), this, SLOT(startWritingSms()));
	connect(b, SIGNAL(clicked()), this, SLOT(removeCurrentPhoneLine()));
}
void	MobiiContactInformations::addMailLine(QString title, QString name)
{
	QListWidgetItem *mail = new QListWidgetItem();
	QLabel *lab = new QLabel(title);
	QLineEdit* t = new QLineEdit(name);
	t->setMinimumWidth(120);
	QPushButton *b = new QPushButton("x");
	b->setMaximumWidth(30);
	QHBoxLayout *l = new QHBoxLayout();
	QWidget *w = new QWidget();
	l->addWidget(lab);
	l->addWidget(t);		
	l->addWidget(b);
	w->setLayout(l);
	mail->setSizeHint(w->sizeHint());
			
	this->ui.mailList->addItem(mail);
	this->ui.mailList->setItemWidget(mail, w);

	connect(b, SIGNAL(clicked()), this, SLOT(removeCurrentMailLine()));
}

void MobiiContactInformations::checkIfLastPhoneLine(QListWidgetItem * current, QListWidgetItem * previous)
{
	if ((this->ui.phoneList->row(current)) == (this->ui.phoneList->count() - 1))
		this->addNewPhoneLine();
}

void MobiiContactInformations::checkIfLastMailLine(QListWidgetItem * current, QListWidgetItem * previous)
{
	if ((this->ui.mailList->row(current)) == (this->ui.mailList->count() - 1))
		this->addNewMailLine();
}
