#include "generator.h"
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
    return true;
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
