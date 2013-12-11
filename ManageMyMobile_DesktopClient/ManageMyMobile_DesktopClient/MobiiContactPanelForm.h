#ifndef MOBIICONTACTPANELFORM_H
#define MOBIICONTACTPANELFORM_H

#include "MobiiContactInformations.h"
#include "ui_MobiiContactPanel.h"
#include "MobiiEventBusCodes.h"
#include "commons.h"
#include "AModuleDependencyUser.h"
#include "ConfigurationFileIo.h"
#include "AExtendedUiWidget.h"

class MobiiContactPanelForm : public QWidget, public AModuleDependencyUser, public AExtendedUiWidget
{
	Q_OBJECT

public:
	explicit MobiiContactPanelForm();
	~MobiiContactPanelForm();

private:
	void					buildDataModel(int i);
	
	// Used for filesystem backup
	typedef QVariantMap					TContactsBackupItem;
	typedef QList<TContactsBackupItem>	TContactsBackupContainer;
	TContactsBackupContainer			rawContactsDatas_;

	ConfigurationFileIo			config_;
	MobiiContactInformations	*contactPanel_;	
	QList<QVariant>			*src_;
	Ui::MobiiContactPanel	ui;
	int						currentContactId_;
	bool					firstTimeInWidget;

private slots:
	void					validateNewContact(const QString, void *, void *);
	void					resetContactList();
	void					updateContactList(QString str);
	void					deleteCurrentContact(const QString eventName, void *eventEmitter, void *data);
	void					updateCurrentContact(const QString eventName, void *eventEmitter, void *data);
	void					newContact();
	void					RetrieveContactsClicked();
	void					SendContactsToMobile();
	void					ContactsReceived(const QString, void *, void *);
	void					contactSelected();
	void					OnDisconnected(const QString, void *, void *);
	void					OnConnected(const QString eventName, void *sender, void *data);
	void					fileReceived(const QString eventName, void *eventEmitter, void *data);
	void					findNameWithPhone(const QString eventName, void *eventEmitter, void *data);

	
protected:
	/// Specifies actions to perform when the event bus is set.
	virtual void	OnEventBusSet(void);
	virtual void	OnWidgetShow(void);
	virtual void	OnWidgetClose(void);
	/// Specifies actions to perform when the session controller is set.
	virtual void OnSessionControllerSet(void);
};



#endif // MOBIICONTACTPANELFORM_H