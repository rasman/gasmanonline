#include "gaslicensemanager.h"
#include "gasapplication.h"
#include <QtDebug>

GasLicenseManager* GasLicenseManager::pinstance = 0;

GasLicenseManager* GasLicenseManager::instance()
{
	if (pinstance == 0)
		pinstance = new GasLicenseManager;
	return pinstance;
}

GasLicenseManager::GasLicenseManager() : QObject(0), validLicense(false)
{
	licOwner = tr("This is an");
	licCompany = tr("Unregistered Version");
	licInfo = tr(" ");
	licKey = tr(" ");
}

void GasLicenseManager::clearRegister()
{
	licOwner = tr("This is an");
	licCompany = tr("Unregistered Version");
	licInfo = tr(" ");
	licKey = tr(" ");
	validLicense = false;

	QSettings settings("Med Man", "Gas Man");

	WriteReg(settings, "Application", "Owner", licOwner);
	WriteReg(settings, "Application", "Company", licCompany);
	WriteReg(settings, "Application", "Info", licInfo);
	WriteReg(settings, "Application", "Key", QString());
}

// read QSettings file
// stores license info locally so it cannot be changed once the application starts
// keeps user from validating the key on startup then changing the name and company later
bool GasLicenseManager::validLicenseExists()
{
	if(validLicense)
		return true;

	QString name, company, info, key;

	{
		QSettings settings("Med Man", "Gas Man");

		name = ReadReg(settings, "Application", "Owner", QString()).toString();
		company = ReadReg(settings, "Application", "Company", QString()).toString();
		info = ReadReg(settings, "Application", "Info", QString()).toString();
		key = ReadReg(settings, "Application", "Key", QString()).toString();
		validLicense =  isLicenseValid(name, company, info, key);
	}

	if ( !validLicense )
	{
		name = gasApp->ReadProfile("License", "Owner", QString()).toString();
		company = gasApp->ReadProfile("License", "Company", QString()).toString();
		info = gasApp->ReadProfile("License", "Info", QString()).toString();
		key = gasApp->ReadProfile("License", "Key", QString()).toString();
		validLicense =  isLicenseValid(name, company, info, key);
	}

	if(validLicense)
	{
		licOwner = name.isEmpty() ? " " : name;
		licCompany = company.isEmpty() ? " " : company;
		licInfo = info.isEmpty() ? " " : info;
		licKey = key.isEmpty() ? " " : key;
	} 
	return validLicense;
}


// algorithm goes here!!
bool GasLicenseManager::isLicenseValid(
	const QString &name, const QString &company, const QString &info, const QString &licenseKey, bool *isGlobal
)
{
	if(name.simplified() == QString() || company.simplified() == QString())
		return false;

	Generator *gen  = new Generator(this);
	gen->setKey(licenseKey);

	bool ret = gen->validate() && gen->getName() == name && gen->getCompany() == company && gen->getInfo() == info;
	if (isGlobal)
		*isGlobal = gen->isGlobal();
	return ret;
}

// if valid license, save in qsettings file
bool GasLicenseManager::attemptRegisterLicense(
	const QString &name, const QString &company, const QString &info, const QString &licenseKey
)
{
	bool global;

	validLicense = false;
	if(isLicenseValid(name, company, info, licenseKey, &global))
	{
		QSettings settings;

		if (global)
		{
			gasApp->WriteProfile("License", "Owner", name);
			gasApp->WriteProfile("License", "Company", company);
			gasApp->WriteProfile("License", "Info", info);
			gasApp->WriteProfile("License", "Key", licenseKey);
		} else {
			QSettings settings("Med Man", "Gas Man");
			WriteReg(settings, "Application", "Owner", name);
			WriteReg(settings, "Application", "Company", company);
			WriteReg(settings, "Application", "Info", info);
			WriteReg(settings, "Application", "Key", licenseKey);
		}

		licOwner = name;
		licCompany = company;
		licInfo = info;
		licKey = licenseKey;
		validLicense = true;
		emit validLicenseRegistered();
	}

	return validLicense;
}

QVariant GasLicenseManager::ReadReg(QSettings& settings, const QString &group, const QString &param, const QVariant &defVal)
{
	QVariant value;

	bool isEmptyGroup = group.isNull();
    if ( !isEmptyGroup ) settings.beginGroup( group );
    value = settings.value( param, defVal );
    if ( !isEmptyGroup ) settings.endGroup();
    return value;
}

void GasLicenseManager::WriteReg(QSettings& settings, const QString &group, const QString &param, const QVariant &val )
{
	bool isEmptyGroup = group.isNull();
	if ( !isEmptyGroup )
		settings.beginGroup( group ); 
	settings.setValue( param, val );
	if ( !isEmptyGroup )
		settings.endGroup();
}
