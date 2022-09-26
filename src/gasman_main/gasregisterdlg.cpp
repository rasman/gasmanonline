#include "gasregisterdlg.h"
#include "gasglobal.h"
#include "gaslicensemanager.h"

#include <QSettings>
#if QT_VERSION >=0x050000
#include <QtWidgets>
#include <QMessageBox>
#else
#include <QtGui>
#endif

/////////////////////////////////////////////////////////////////////////////
// GasRegisterDlg dialog used for App Registration

GasRegisterDlg::GasRegisterDlg( QWidget *parent )
	: QDialog( parent )
{
	setupUi( this );

	//Remove a context help button from dialogs
	setWindowFlags( windowFlags() ^ Qt::WindowContextHelpButtonHint );

	
	connect(buttonBox->button(QDialogButtonBox::Ok), SIGNAL(clicked()),
					this, SLOT(attemptRegisterLicense()));
	connect(openFile,SIGNAL(clicked()),this, SLOT(registerFromFile()));

	connect(pbTemp, SIGNAL(clicked()), this, SLOT(cleanRegister()));
	pbTemp->setVisible(false);
	pbTemp->setText("Clear Reg");
}

void GasRegisterDlg::cleanRegister()
{
	glm->instance()->clearRegister();
}

void GasRegisterDlg::attemptRegisterLicense()
{
	if(	glm->instance()->attemptRegisterLicense(
			nameLineEdit->text().simplified(),
			companyLineEdit->text().simplified(),
			infoLineEdit->text().simplified(),
			keyLineEdit->text().simplified()
		)
	)
	{
		QMessageBox::warning(this, tr("GasMan"),
												 tr("GasMan is now registered and has changed to full mode."),
												 QMessageBox::Ok);
		accept();
	}else{
		QMessageBox::warning(this, tr("GasMan"),
												 tr("<html><body>The license key you provided was not valid.<br>Please enter the correct information or purchase a license from <A HREF=""http://www.gasmanweb.com"">http://www.gasmanweb.com</A>.</body></html>"),
												 QMessageBox::Ok);
	}
}

void GasRegisterDlg::registerFromFile()
{
	QString add = QFileDialog::getOpenFileName(this, tr("Open File"),
					QString(),tr("license file(*.gls)"));
	QFile file(add);
	if(file.open(QIODevice::ReadOnly)){
		QTextStream st(&file);
		QString name = file.readLine();
		QString company = file.readLine();
		QString info = file.readLine();
		QString  lic = file.readLine();
		
		nameLineEdit->setText(name);
		companyLineEdit->setText(company);
		infoLineEdit->setText(info);
		keyLineEdit->setText(lic);
		file.close();
	}
}