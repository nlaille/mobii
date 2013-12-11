#include "MobiiLegalNoticeDialog.h"


MobiiLegalNoticeDialog::MobiiLegalNoticeDialog(void)
{
	this->ui.setupUi(this);
	this->initialize();

	// Bindings
	this->connect(this->ui.closeBtn, SIGNAL(clicked()), this, SLOT(close()));
}


MobiiLegalNoticeDialog::~MobiiLegalNoticeDialog(void)
{
}


void		MobiiLegalNoticeDialog::initialize(void)
{
	this->ui.legalNoticeField->setHtml(
		QString("<html>Mobii<br/>")
		+ QString("Copyright© 2012-2013 - contributeurs Mobii<br/><br/>")
		+ QString("Le projet Mobii est distribué gratuitement. Vous êtes libre de modifier et de diffuser cette application tant que les noms de l'équipe des contributeurs Mobii sont indiqués de façon explicite et suffisemment visible.<br/>")
		+ QString("<br/>Equipe des contributeurs Mobii (dans l'ordre alphabéque) :")
		+ QString("<ul><li>AUBERT Geoffroy</li>")
		+ QString("<li>DE-THORE Nicolas</li>")
		+ QString("<li>GARDET Camille</li>")
		+ QString("<li>GUILLERM Sébastien</li>")
		+ QString("<li>LAILLE Nicolas</li>")
		+ QString("<li>LE Pierre</li>")
		+ QString("<li>MCKENNA Lucas</li>")
		+ QString("<li>MORISSARD Clément</li></ul>")
		+ QString("<br/><br/>Cette application utilise des composants tiers :")
		+ QString("<br/><ul><li><b>Glyphicons</b>")
		+ QString("<br/>http://www.glyphicons.com/")
		+ QString("<br/>Glyphicons est fournie sous license CC-BY (Creative Commons Attribution 3.0 Unported).</li></ul>")
		+ QString("</html>")
		);
}