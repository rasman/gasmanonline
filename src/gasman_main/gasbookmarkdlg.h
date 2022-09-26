
#ifndef GASBOOKMARKDLG_H
#define GASBOOKMARKDLG_H

#include "ui_gasbookmarkdlg.h"
#include "gasglobal.h"

class GasDoc;

class GasBookmarkDlg : public QDialog, private Ui::GasBookmark
{
	class TargetListWidgetItem : public QListWidgetItem
	{
	public:
		TargetListWidgetItem(const AResult compartment, const int percent)
			: m_compartment(compartment), m_percent(percent), QListWidgetItem(toQString(compartment, percent)) {}
		virtual bool operator<(const QListWidgetItem &other) const
		{
			const TargetListWidgetItem& o = *dynamic_cast<const TargetListWidgetItem*>(&other);
			return m_compartment < o.m_compartment
                || (m_compartment == o.m_compartment && m_percent < o.m_percent);
		}
		AResult getCompart() const { return m_compartment; }
		int getPercent() const { return m_percent; }
		static QString toQString(const QString compartment, int percent)
		{
			return QString("%1 %2 %3%").arg(compartment, tr("at"), QString::number(percent).rightJustified(3));
		}
	private:
		QString toQString(const AResult compartment, const int percent) const { return toQString(listCompart[compartment], percent); }
		AResult m_compartment;
		int m_percent;
	};

	Q_OBJECT
public:
	GasBookmarkDlg( QWidget *parent = 0 );
	void setHighTime( quint32 max );
	void setTime( quint32 time );
	inline void setDoc( GasDoc *doc ) { m_pDoc = doc; }

public slots:
	int exec();					//initialize and shows the dialog as a modal dialog
	void addBookmark();
	void addTarget();
	void deleteBookmark();
	void deleteTarget();
	void deleteAllBookmarks();
	void deleteAllTargets();
	void saveBookmarks();

	void defaultBookmark();
	void defaultTarget();
	void defaultOk();

protected:
	bool eventFilter(QObject * object, QEvent * event);

private:
	GasDoc *m_pDoc;					//pointer to the main document
	bool m_bFull;
	int m_nHHMax;
	int m_nMMMax;
	int m_nSSMax;
};

#endif
