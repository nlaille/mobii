#ifndef MOBIICONTACTITEM_H
#define MOBIICONTACTITEM_H

#include <qdatetime.h>
#include <qstring.h>

class MobiiContactItem
{
public:
	MobiiContactItem();
	MobiiContactItem(int id, QString fn, QString ln, QString n, QDate b, QString mP, QString mPr, QString mailP, QString mailPr, QString s);
	~MobiiContactItem();

	int			id;
	QString		firstname;
	QString		lastname;
	QString		notes;
	QDate		birthday;
	QString		mobilePerso;
	QString		mobilePro;
	QString		mailPerso;
	QString		mailPro;
	QString		society;

private:
	
};

#endif // MOBIICONTACTITEM_H
