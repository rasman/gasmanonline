#include "assistant.h"
#include <QApplication>
#include <QProcess>
#include <QLibraryInfo>
#include <QDir>
#include <QMessageBox>
#include <QDebug>
Assistant::Assistant(void)
{
	proc = NULL;
}

Assistant::~Assistant(void)
{
	if (proc && proc->state() == QProcess::Running) {
		proc->terminate();
		proc->waitForFinished(3000);
	}
	delete proc;
}

bool Assistant::startAssistant()
{
	if (!proc)
		proc = new QProcess();

	if (proc->state() != QProcess::Running) {
		QString app = qApp->applicationDirPath() + QDir::separator();
#if !defined(Q_OS_MAC)
		app += QLatin1String("assistant");
#else
		app += QLatin1String("Assistant.app/Contents/MacOS/Assistant");
#endif

		QStringList args;
#if defined(Q_OS_MAC)
		args	<< QLatin1String("-collectionFile")
				<< qApp->applicationDirPath() + QDir::separator() +
                                      QLatin1String("..") + QDir::separator() + QLatin1String("Resources") + QDir::separator() + QLatin1String("GasMan.qhc")
				<< QLatin1String("-enableRemoteControl");
#else
                args    << QLatin1String("-collectionFile")
                                << qApp->applicationDirPath() + QDir::separator() + QLatin1String("GasMan.qhc")
                                << QLatin1String("-enableRemoteControl");
#endif
		qDebug()<<args;
		proc->start(app, args);

		if (!proc->waitForStarted()) {
			QMessageBox::critical(0, QObject::tr("Simple Text Viewer"),
			QObject::tr("Unable to launch Qt Assistant (%1)").arg(app));
			return false;
		}
	}

	return true;
}

void Assistant::showDocumentation(const QString &page)
{
	if (!startAssistant())
		return;

	QByteArray ba("SetSource ");
	ba.append("qthelp://help.gasman.gasmanweb.com/gasman/");

	proc->write(ba + page.toLocal8Bit() + '\0');
 }
