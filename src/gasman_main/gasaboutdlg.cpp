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

	QString version = "<p>" + gasmanVersion;
	if(!glm->instance()->validLicenseExists())
		version += tr("<br>Student Version" );
	version += "</p>";

	lText->setText( QApplication::translate("GasAboutDlg", "<html><body> \
						<center><p><strong>Gas Man&reg;<br/>Understanding Anesthesia Uptake and Distribution</strong></p></center> \
						<center>%1</center> \
						<center><p>By James H. Philip, M.E. (E.), M.D.<br/>Brigham and Women's Hospital<br/>Harvard Medical School</p></center> \
						<center><p>Software designed and written by:<br/>H M Franklin Associates, San Ramon, CA.<br/> \
						<a href=""http://www.hmfa.com"">http://www.hmfa.com</a></p></center> \
						<center><p>Qt Implementation by:<br/>Integrated Computer Solutions, Inc.<br/> \
						<a href=""http://www.ics.com"">http://www.ics.com</a></p></center> \
						<center><p>Med Man Simulations, Inc.<br/>A Nonprofit Corporation<br/>P O Box 67-160, Chestnut Hill, MA 02467<br/>Phone 617 277-2117<br/> \
						<a href=""http://www.gasmanweb.com"">http://www.gasmanweb.com</a></p></center> \
						<center><p>Copyright&copy;1984-2016 James H. Philip<br/>All Rights Reserved</p></center> \
						<center><p>Licensed To:<br/>%2<br/>%3</p></center> \
						</body></html>" ).arg(version,  glm->instance()->licOwner ,glm->instance()->licCompany));//.arg( glm->instance()->validLicenseExists()? tr("Never"): gasApp->expireDate.toString("yyyy-MM-dd")));
		//<p><center>Expire Date:<br> %4</center></p>
	lText->setOpenExternalLinks( true );

	//Create connections
	connect( buttonBox, SIGNAL(accepted()), this, SLOT(accept()) );
}
