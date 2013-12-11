#ifndef MOBIISMSSTREAMITEM_H_
#define MOBIISMSSTREAMITEM_H_

#include		<qstring.h>
#include		<qdatetime.h>

#include		"ui_MobiiSmsStreamItem.h"

/*! \file */

/// \brief Represents a single discussion stream in the preview area on the left of the SMS panel.
class		MobiiSmsStreamItem : public QWidget
{
	Q_OBJECT

public:
	/// Initializes a stream header with a sender number, the last message sent to or received from the recipient, and the date it was received.
	explicit MobiiSmsStreamItem(
		const QString &senderNb,
		const QString &lastMessage, 
		const QDateTime &lastDate);
	virtual ~MobiiSmsStreamItem(void);

	/// SMS recipient phone number
	void	setSenderNb(const QString &senderNb);

	/// Last message send to or received from the recipient
	void	setLastMessage(const QString &message);

	/// Date when the last message was received
	void	setLastDate(const QDateTime &date);

	/// \brief Name of the recipient based on the raw phone number.
	/// Availability of this information relies on the contacts list of the phone.
	void	setConvenientSenderName(const QString &name);

	/// SMS recipient phone number
	const QString &	senderNb(void) const;

	/// Last message send to or received from the recipient
	const QString	senderText(void) const;

private:
	/// Stores the sender's actual phone number, in case it is replaced by a convenient name.
	QString					m_senderNb;
	Ui::MobiiSmsStreamItem	ui;
};


#endif // !MOBIISMSSTREAMITEM_H_