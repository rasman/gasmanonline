#include "gasaboutdlg.h"
#include "gasapplication.h"
#include "gaslicensemanager.h"
#include "gasglobal.h"

/////////////////////////////////////////////////////////////////////////////
// GasAboutDlg dialog used for App About

GasAboutDlg::GasAboutDlg( QWidget *parent )
	: QDialog( parent )
{
	setupUi( this );
	setAttribute( Qt::WA_DeleteOnClose );
	//Remove a context help button from dialogs
	setWindowFlags( windowFlags() ^ Qt::WindowContextHelpButtonHint );

#ifdef Q_OS_WASM
	// ── WASM / Web Edition About text ────────────────────────────────────
	lText->setText(QString(
		"<html><body>"
		"<center><p><strong>Gas Man&reg;<br/>"
		"Understanding Anesthesia Uptake and Distribution</strong></p></center>"
		"<center><p>Version %1 &mdash; Web Edition</p></center>"
		"<center><p>By James H. Philip, M.E.(E.), M.D.<br/>"
		"Brigham and Women's Hospital &bull; Harvard Medical School</p></center>"
		"<center><p>Med Man Simulations, Inc.<br/>"
		"A 501(C)(3) Charitable Organization<br/>"
		"<a href=\"http://www.gasmanweb.com\">http://www.gasmanweb.com</a></p></center>"
		"<center><p>Copyright &copy; 1984&ndash;2016 James H. Philip<br/>"
		"Licensed under <strong>GPL v3</strong></p></center>"
		"<center><p>WASM development lead by:<br/>"
		"<strong>Elie Sarraf, M.D., C.M.</strong></p></center>"
		"<center><p>Source code:<br/>"
		"<a href=\"https://github.com/rasman/gasmanonline\">"
		"https://github.com/rasman/gasmanonline</a></p></center>"
		"</body></html>").arg(gasmanVersion));
#else
	// ── Desktop About text ───────────────────────────────────────────────
	QString version = "<p>" + gasmanVersion;
	if (!glm->instance()->validLicenseExists())
		version += tr("<br>Student Version");
	version += "</p>";

	lText->setText(QApplication::translate("GasAboutDlg",
		"<html><body>"
		"<center><p><strong>Gas Man&reg;<br/>Understanding Anesthesia Uptake and Distribution</strong></p></center>"
		"<center>%1</center>"
		"<center><p>By James H. Philip, M.E. (E.), M.D.<br/>Brigham and Women's Hospital<br/>Harvard Medical School</p></center>"
		"<center><p>Software designed and written by:<br/>H M Franklin Associates, San Ramon, CA.<br/>"
		"<a href=\"http://www.hmfa.com\">http://www.hmfa.com</a></p></center>"
		"<center><p>Qt Implementation by:<br/>Integrated Computer Solutions, Inc.<br/>"
		"<a href=\"http://www.ics.com\">http://www.ics.com</a></p></center>"
		"<center><p>Med Man Simulations, Inc.<br/>A Nonprofit Corporation<br/>"
		"P O Box 67-160, Chestnut Hill, MA 02467<br/>Phone 617 277-2117<br/>"
		"<a href=\"http://www.gasmanweb.com\">http://www.gasmanweb.com</a></p></center>"
		"<center><p>Copyright &copy; 1984&ndash;2016 James H. Philip<br/>All Rights Reserved</p></center>"
		"<center><p>Licensed To:<br/>%2<br/>%3</p></center>"
		"</body></html>").arg(version, glm->instance()->licOwner, glm->instance()->licCompany));
#endif // Q_OS_WASM
	lText->setOpenExternalLinks(true);

	//Create connections
	connect( buttonBox, SIGNAL(accepted()), this, SLOT(accept()) );
}
