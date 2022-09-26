#ifndef GASLICENSEMANAGER_H
#define GASLICENSEMANAGER_H

#include <QtCore>
#include "generator.h"
// Singleton GasLicenseManager
class GasLicenseManager : public QObject
{
	Q_OBJECT

public:
	static GasLicenseManager* instance();

	bool validLicenseExists();
	bool isLicenseValid(
		const QString &name, const QString &company,const QString &info, const QString &licenseKey, bool *isGlobal = NULL
		);
	bool attemptRegisterLicense(
		const QString &name, const QString &company, const QString &info, const QString &licenseKey
		);
	void clearRegister();

	QString licOwner;
	QString licCompany;
	QString licInfo;
	QString licKey;

signals:
	void validLicenseRegistered();

protected:
	GasLicenseManager();
	GasLicenseManager(const GasLicenseManager&);
	GasLicenseManager& operator= (const GasLicenseManager&);

private:
	QVariant ReadReg(QSettings&  settings, const QString &group, const QString &param, const QVariant &defVal);
	void WriteReg(QSettings&  settings, const QString &group, const QString &param, const QVariant &val );
	bool validLicense;
	static GasLicenseManager* pinstance;
};

extern GasLicenseManager* glm;

#endif
