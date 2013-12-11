#ifndef MOBIICONTACTINFORMATIONS_H
#define MOBIICONTACTINFORMATIONS_H

#include <qdialog.h>
#include <qevent.h>
#include "AModuleDependencyUser.h"
#include "ui_MobiiContactInformations.h"
#include "ConfigurationFileIo.h"
#include "MobiiNewSmsDialog.h"

class MobiiContactInformations : public QDialog, public AModuleDependencyUser
{
	Q_OBJECT

public:
	explicit MobiiContactInformations(QVariantMap *contactData);

	void setContactData(QVariantMap *contactData);
	~MobiiContactInformations();

private:
	void	addPhoneLine(QString title, QString name);
	void	addMailLine(QString title, QString name);

	ConfigurationFileIo			config_;
	QString						currentID_;
	Ui::MobiiContactInformations ui;
private slots:

	void	startWritingSms();
	void	addNewPhoneLine();
	void	addNewMailLine();
	void	removeCurrentPhoneLine();
	void	removeCurrentMailLine();

	void	updateCurrentContact();
	void	deleteCurrentContact();
	void	checkIfLastPhoneLine(QListWidgetItem * current, QListWidgetItem * previous);
	void	checkIfLastMailLine(QListWidgetItem * current, QListWidgetItem * previous);

protected:
	virtual void closeEvent(QCloseEvent *event);
	/// Specifies actions to perform when the event bus is set.
	virtual void OnEventBusSet(void);
	/// Specifies actions to perform when the session controller is set.
	virtual void OnSessionControllerSet(void);
};

#endif // MOBIICONTACTINFORMATIONS_H
