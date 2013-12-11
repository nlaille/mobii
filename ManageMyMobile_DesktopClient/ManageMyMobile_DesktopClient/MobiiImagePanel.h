#ifndef MobiiFilePanel_H
#define MobiiFilePanel_H

#include <QWidget>
#include <qdatetime.h>
#include "ui_MobiiImagePanel.h"
#include "MobiiEventBusCodes.h"
#include "AModuleDependencyUser.h"
#include "AExtendedUiWidget.h"
#include "ConfigurationFileIo.h"

class MobiiFilePanel : public QWidget, public AModuleDependencyUser, public AExtendedUiWidget
{
	Q_OBJECT

public:
	MobiiFilePanel(QWidget *parent = 0);
	~MobiiFilePanel();

private:
	unsigned int		currentImg_;
	QList<QVariant>		*src_;
	Ui::MobiiImagePanel ui;
	ConfigurationFileIo	config_;

	bool				firstTimeInWidget;

	// Used for filesystem backup
	typedef QVariantMap					TImagesBackupItem;
	typedef QList<TImagesBackupItem>	TImagesBackupContainer;
	TImagesBackupContainer				rawImagesDatas_;

	void			requestFiles();
	void			resetImageList();

private slots:
	void			fileListReceived(const QString eventName, void *eventEmitter, void *data);
	void			fileReceived(const QString eventName, void *eventEmitter, void *data);
	void			OnDisconnected(const QString, void *, void *);
	void			OnConnected(const QString, void *, void *);
	void			deleteFile();
	void			saveFile();
	void			fileSelected(const QItemSelection &, const QItemSelection &);

protected:
	virtual void	OnWidgetShow(void);
	virtual void	OnWidgetClose(void);
	/// Specifies actions to perform when the event bus is set.
	virtual void OnEventBusSet(void);
	/// Specifies actions to perform when the session controller is set.
	virtual void OnSessionControllerSet(void);
};

#endif // MobiiFilePanel_H
