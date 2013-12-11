#ifndef MOBIILEGALNOTICEDIALOG_H_
#define MOBIILEGALNOTICEDIALOG_H_

#include <qdialog.h>

#include "ui_MobiiLegalNoticeDialog.h"
#include "MobiiEventBusCodes.h"
#include "AModuleDependencyUser.h"

/*! \file */

/// \brief Dialog which shows informations regarding copyrighting and legal requirements.
class MobiiLegalNoticeDialog: public QDialog, public AModuleDependencyUser
{
	Q_OBJECT

public:
	MobiiLegalNoticeDialog(void);
	virtual ~MobiiLegalNoticeDialog(void);

private:
	Ui::MobiiLegalNoticeDialog	ui;

	void		initialize(void);
};

#endif // !MOBIILEGALNOTICEDIALOG_H_