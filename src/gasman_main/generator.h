#ifndef GENERATOR_H
#define GENERATOR_H

#include <QObject>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QThread>
class Generator : public QObject
{
    Q_OBJECT

public:
    Generator(QObject *parent = 0);
    ~Generator();

    bool toFile( QFile & file);
    void generate();
    QString getLicense();

    void setPlat(const QString & plat) {platform = plat;}
    void setName(const QString & n) {name = n;}
    const QString& getName() {return name;}
    void setCompany(const QString & c){company = c;}
    const QString& getCompany() {return company;}
    void setInfo(const QString & c){exInfo = c;}
    const QString& getInfo() {return exInfo;}
    void setDemoMode(const bool & d) { demo = d;}
    void setGlobalMode(const bool & g) { global = g; }

    void setKey(const QString & s){key = s;}
    bool validate();
    bool isGlobal() const { return global; }
    void setDays(const int &days) { date = QDateTime::currentDateTime().addDays(days).toString("yyyyMMdd"); }

private:
    QString platform;
    QString name;
    QString company;
    QString exInfo;
    QString date;

    bool demo, global;

    QString key, license;

    class ValidationThread : public QThread
    {
    public:
        ValidationThread(Generator *parent);
        bool validate();

    protected:
        virtual void run();

    private:
        bool checkValidation();
        bool m_valid;
        Generator* m_parent;
    };
};

#endif // GENERATOR_H
