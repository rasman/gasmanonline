#include "generator.h"
#include <Qca-qt5/QtCrypto/QtCrypto>
#include <Qca-qt5/QtCrypto/qca.h>
#include <QtDebug>

#include "import_plugins.h"

Generator::ValidationThread::ValidationThread(Generator *parent)
    : QThread(parent), m_parent(parent)
{
}

bool Generator::ValidationThread::validate()
{
    m_valid = false;
    start();
    wait();
    return m_valid;
}

void Generator::ValidationThread::run()
{
    m_valid = checkValidation();
}

bool Generator::ValidationThread::checkValidation()
{
    QCA::Initializer init;
    if(!QCA::isSupported("pkey") ||!QCA::PKey::supportedIOTypes().contains(QCA::PKey::RSA))
        return false;

    QStringList strings = (m_parent->key + "|||||").split('|');
    QCA::Base64 decode(QCA::Direction::Decode);
    QByteArray candidateSignature = decode.stringToArray(strings[0]).toByteArray();

    strings = strings.mid(1,5);
    QString payload = strings.join("|");

    QCA::SecureArray message = payload.toLatin1();

    QCA::ConvertResult result;
    QCA::PublicKey pubkey = QCA::PublicKey::fromPEM(
        "-----BEGIN PUBLIC KEY-----\n"
        "MIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQDpmRKWCronYfwzD+OwwxF1ol7p\n"
        "fsNoz+XUqVnB2NXcT8K3t1GU9U51uzpoUsh9+PimW72mGhq4/HQ/MYHvQz+bfdS/\n"
        "7OcAgGkfoCPvNzz+U6hRKAFzFuDnXUOCjHL2v6RnKCLNh/DR246vgRNY+Nf2Hujz\n"
        "D9A2OYE4NkDhNlkWYwIDAQAB\n"
        "-----END PUBLIC KEY-----\n",
                &result
        );
    if (QCA::ConvertGood != result)
    {
            qDebug() << "Public key read failed\n";
            return false;
    }

	QCA::SignatureAlgorithm algorithm = message.size() > 116
		? QCA::EMSA3_SHA1
		: QCA::EMSA3_Raw;
	if ( !pubkey.verifyMessage( message, candidateSignature, algorithm ) )
        return false;

    for( int i=0; i<5; ++i)
    {
        QByteArray a = decode.stringToArray(strings[i]).toByteArray();
        strings[i] = QString::fromLatin1(a, a.size());
    }

    bool lensOK = strings[0].length() == 2 && strings[3].length() == 8;

    m_parent->setGlobalMode(lensOK && strings[0].at(0).isUpper());
    m_parent->setDemoMode(!lensOK || strings[0].at(1) != 'z');
    m_parent->setPlat(lensOK ? strings[0].at(0).toLower() : 'u');

    m_parent->name = strings[1];
    m_parent->company = strings[2];
    m_parent->exInfo = strings[4];

    if( m_parent->demo ) {
        m_parent->date = strings[3];
        QDate d = QDate::fromString(m_parent->date, "yyyyMMdd");
        if( QDate::currentDate().daysTo(d) <= 0 )
            return false;
    }

#ifdef Q_OS_MACX
    QString plat = "m";
#else
    #ifdef Q_OS_LINUX
            QString plat = "l";
        #else
            QString plat = "w";
    #endif
#endif
    return plat == m_parent->platform;
}

Generator::Generator(QObject *parent)
    : QObject(parent)
{
}

Generator::~Generator(){}

bool Generator::validate()
{
    ValidationThread t(this);
    return t.validate();
}

void Generator::generate()
{
    license.clear();
}

QString Generator::getLicense()
{
    return license;
}

bool Generator::toFile(QFile & /*file*/)
{
    return false;
}
