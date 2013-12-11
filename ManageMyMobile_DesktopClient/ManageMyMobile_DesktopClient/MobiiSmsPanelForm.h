#ifndef MOBIISMSPANELFORM_H_
#define MOBIISMSPANELFORM_H_

#include <qdatastream.h>
#include <qdatetime.h>
#include <qstandarditemmodel.h>
#include <qmap.h>
#include <qlist.h>
#include <qclipboard.h>

#include "ui_MobiiSmsPanel.h"
#include "MobiiNewSmsDialog.h"
#include "AModuleDependencyUser.h"
#include "AExtendedUiWidget.h"

/*! \file */

/// \brief Panel which allows reading SMS streams and writing new messages.
class MobiiSmsPanelForm : public QWidget, public AModuleDependencyUser, public AExtendedUiWidget
{
	Q_OBJECT

public:
	MobiiSmsPanelForm(void);
	virtual ~MobiiSmsPanelForm(void);

private:
	/// Column index for the message content, in the JSON table received from the mobile.
	static const int C_ItemContentIdx = 0;

	/// Column index for the date when the message was received, in the JSON table received from the mobile.
	static const int C_ItemDateIdx = 1;

	/// Column index for the flag which tells if the message was received from or sent to the recipient.
	static const int C_ItemSentByPhoneIdx = 2;

	/// P1: send date, P2: content
	typedef QString						TSmsSender;
	typedef QList<QVariant>				TSmsItem;
	typedef QList<TSmsItem>				TSmsItemList;

	/// Key: sender phone number as string
	/// Values: list of single SMS
	typedef QMap<TSmsSender, TSmsItemList>	TSmsSenderList;

	/// Stores the date when the last SMS was received from the phone.
	QDateTime				m_lastReceivedDate;

	/// Internal model which stores all SMS by sender
	TSmsSenderList			m_streams;

	/// New SMS dialog handle
	MobiiNewSmsDialog *		m_newSmsDialog;
	Ui::MobiiSmsPanel		ui;

	/// Current offset in the messages stream
	quint32					m_streamOffset;

	/// Stores the currently displayed discussion stream (phone number)
	QString					m_currentSenderDisplayed;

	bool					m_synchronizeOnNextTabSelection;

	/// Updates the streams list (by sender)
	void			updateStreamsListOnUi(void);

	/// Returns an uniformized caller phone number for comparisons.
	QString			getUniformizedSenderNb(const QString &nb);
	
	/// Gets/sets the date of the last received message for synchronization.
	QDateTime		getDateOfLastMessage(void);
	void			setDateOfLastMessage(void);

	/// Used as algorithm brick for qSort()
	static bool		smsDateComparison(const TSmsItem &a, const TSmsItem &b);

	/// Creates the `load more' button
	void			makeLoadMoreButton(void);

	/// Returns a senders list ordered by the date of last received SMS
	QList<QString>	getListOfSendersOrderedByLastReceived(QList<QString>);

	virtual void	OnWidgetShow(void);

private slots:
	void			OnRetrieveSmsClicked(void);
	void			OnWriteNewSmsClicked(void);
	void			OnSmsReceived(const QString eventName, void *sender, void *data);
	void			OnConnected(const QString eventName, void *sender, void *data);
	void			OnDisconnected(const QString eventName, void *sender, void *data);

	/// Shows the messages stream for a given user
	void			displayStreamForSenderOnUi(void);

	// List slots
	void			OnStreamSelectionChanged(QListWidgetItem *curr, QListWidgetItem *prev);
	void			OnPhoneNbMatchFound(const QString eventName, void *sender, void *data);

	// Discussion stream slots
	void			OnDiscussionStreamItemClicked(int row, int column);
	void			OnDiscussionStreamContextMenu(const QPoint &point);
	void			OnCopyMessageRequested(bool);

	// Answer box
	void			OnReplyBtnClicked(void);

protected:
	/// Specifies actions to perform when the event bus is set.
	virtual void OnEventBusSet(void);
};

#endif // !MOBIISMSPANELFORM_H_