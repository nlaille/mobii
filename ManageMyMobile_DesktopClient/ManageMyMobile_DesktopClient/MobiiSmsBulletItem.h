#ifndef MOBIISMSBULLETITEM_H_
#define MOBIISMSBULLETITEM_H_

#include	<qstring.h>
#include	<qdatetime.h>

#include	"ui_MobiiSmsBulletItem.h"

/*! \file */

/// \brief Graphical representation of a bullet in a discussion stream in the SMS panel.
class			MobiiSmsBulletItem : public QWidget
{
	Q_OBJECT

public:
	MobiiSmsBulletItem(void);

	/// Initializes a bullet with a content and its associated date
	MobiiSmsBulletItem(const QString &content, const QDateTime &datetime);
	virtual ~MobiiSmsBulletItem(void);

	/// Content of the bullet, or message
	const QString		content(void) const;

	/// Date when the message was sent or received
	const QString		datetime(void) const;

	/// Content of the bullet, or message
	void				setContent(const QString &);

	/// Date when the message was sent or received
	void				setDatetime(const QDateTime &);

	/// \brief Sets the color of the bullet depending on the bullet mode.
	/// - Mode 1 stands for "message received from recipient"
	/// - Mode 2 stands for "message sent by the user"
	void				setColorMode(int mode);

private:
	Ui::SmsBulletItem	ui;
};


#endif // !MOBIISMSBULLET_H_