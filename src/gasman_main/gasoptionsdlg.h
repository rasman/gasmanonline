
#ifndef GASOPTIONSDLG_H
#define GASOPTIONSDLG_H

#include <QDialog>
#include "ui_gaspatient.h"
#include "ui_gaspdefaults.h"
#include "ui_gasvdefaults.h"
#include "ui_gassdefaults.h"
#include "ui_gaschoicesound.h"
#include "ui_gascdefaults.h"
#include "gasglobal.h"

/////////////////////////////////////////////////////////////////////////////
// GasPatientWidget

class GasPatientWidget : public QWidget, private Ui::GasPatient
{
	Q_OBJECT
public:
	GasPatientWidget( QWidget *parent = 0 );

	inline void setInitialState( bool state ) { m_bInitial = state; }
	inline bool initialState() const { return m_bInitial; }

	void setWeight( double w );
	double weight() const;

	void setVolumeALV( double alv );
	double volumeALV() const;

	void setVolumeVRG( double vrg );
	double volumeVRG() const;

	void setVolumeMUS( double mus );
	double volumeMUS() const;

	void setVolumeFAT( double fat );
	double volumeFAT() const;

	void setVolumeVEN( double ven );
	double volumeVEN() const;

	void setRatioVRG( int vrg );
	int ratioVRG() const;

	void setRatioMUS( int mus );
	int ratioMUS() const;

	void setRatioFAT( int fat );
	int ratioFAT() const;

	void setCO( double co );
	double CO() const;
	double NormalCO() const;

	void setVA( double va );
	double VA() const;
	double NormalVA() const;

public slots:
	void weightUnitChanged( int id );
	void weightChanged();
	void metabolismVaChanged();
	void metabolismCoChanged();
	void alvChanged();
	void vrgChanged();
	void musChanged();
	void fatChanged();
	void venChanged();

private:
	bool checkRatio();
	void init();				//Initialization
	void setInternalValue(double& iv, const QDoubleSpinBox* const pSB) const;
	inline double round( const double value, const double roundingValue) const {return floor(value * roundingValue + 0.5) / roundingValue;}
	inline double roundingValue(const QDoubleSpinBox* const pSB) const {return pow( 10.0, pSB->decimals() );} 

private:
	void ComputeNormalizationFactors();
	QButtonGroup *bgWeight;
	bool m_bInitial;
	double m_fScale;
	double m_fFlowFactor;
	double m_fVolumeFactor;
	double m_fMin;
	double m_fMax;
	int cur_id;

	double m_VA, m_CO, m_ALV, m_VRG, m_MUS, m_FAT, m_VEN;

private:
	friend class GasPatientDlg;
	friend class GasOptionsDialog;
};


/////////////////////////////////////////////////////////////////////////////
// GasPDefaultsWidget

class GasChoiceSound : public QDialog, private Ui::GasChoiceSound
{
	Q_OBJECT
public:
	GasChoiceSound( QWidget *parent = 0 );

public slots:
	virtual void accept();
	int exec();		//Initialize and show the dialog as a modal dialog

private slots:
	void edited();
	void browse();
};

class GasPDefaultsWidget : public QWidget, private Ui::GasPDefaults
{
	Q_OBJECT
public:
	GasPDefaultsWidget( QWidget *parent = 0 );

	void setSignal( int sig );
	int signal() const;

	void setLineLabel( int label );
	int lineLabel() const;

	void setLineThickness( int thickness );
	int lineThickness() const;

	void setJumpPercent( int percent );
	int jumpPercent() const;

	void setUseGraphpaper( bool use );
	bool isUseGraphpaper() const;

private slots:
	void graphPercentChanged( int value );
	void custom();

private:
	QButtonGroup *bgSignal;
	QButtonGroup *bgLineLabel;
	QButtonGroup *bgLineThickness;
};


/////////////////////////////////////////////////////////////////////////////
// GasVDefaultsWidget

class GasVDefaultsWidget : public QWidget, private Ui::GasVDefaults
{
	Q_OBJECT
public:
	GasVDefaultsWidget( QWidget *parent = 0 );

	void setEnabledScroll( bool set );
	bool isEnabledScroll() const;

	void setVisibleCost( bool show );
	bool isVisibleCost() const;

	void setVisibleMl( bool show );
	bool isVisibleMl() const;

	void setVisibleControl( bool show );
	bool isVisibleControl() const;

	void setHighCO( quint16 hCO );
	quint16 highCO() const;

	void setHighFGF( quint16 hFGF );
	quint16 highFGF() const;

	void setHighVA( quint16 hVA );
	quint16 highVA() const;

	void setScale( const QString &scal );
	QString scale() const;
};

/////////////////////////////////////////////////////////////////////////////
// GasCDefaultsWidget

class GasCDefaultsWidget : public QWidget, private Ui::GasCDefaults
{
	Q_OBJECT

public:
	GasCDefaultsWidget(QWidget* parent = 0);
	GasAttributeMap getSizes() const;
	GasAttributeMap getCosts() const;
	GasAttributeMap getColors() const;
	void addAgentSizeCostColor(const QString& agent, const int& size, const float& cost, const QString& color, const bool editable = true);
};

/////////////////////////////////////////////////////////////////////////////
// GasSDefaultsWidget

class GasSDefaultsWidget : public QWidget, private Ui::GasSDefaults
{
	Q_OBJECT
public:
	GasSDefaultsWidget( QWidget *parent = 0 );

	void setVolumeCKT( double ckt );
	double volumeCKT() const;

	void setFGF( double fgf );
	double FGF() const;

	void setSpeed( const QString &spd );
	QString speed() const;

	inline void setAgent( const QString &agnt ) { m_agent = agnt; }
	QString agent() const;

	void setCircuit( const QString &circ );
	QString circuit() const;

private:
	void init();		//Initialization

private:
	QString m_agent;

private:
	friend class GasOptionsDialog;
};

//////////////////////////////////////////////////////////////////////
///// GasOptionsDialog

class QTabWidget;

class GasOptionsDialog : public QDialog
{
	Q_OBJECT
public:
	GasOptionsDialog( QWidget *parent = 0 );
	
	inline GasPatientWidget *patientParameters() const { return gasPatientWidget; }
	inline GasPDefaultsWidget *programDefaults() const { return gasPDefaultsWidget; }
	inline GasVDefaultsWidget *viewDefaults() const { return gasVDefaultsWidget; }
	inline GasSDefaultsWidget *simulationDefaults() const { return gasSDefaultsWidget; }
	inline GasCDefaultsWidget *colorDefaults() const { return gasCDefaultsWidget; }

public slots:
	virtual void accept();
	int exec();					//Initialize and show the dialog as a modal dialog

private:
	GasPatientWidget *gasPatientWidget;
	GasPDefaultsWidget *gasPDefaultsWidget;
	GasVDefaultsWidget *gasVDefaultsWidget;
	GasSDefaultsWidget *gasSDefaultsWidget;
	GasCDefaultsWidget *gasCDefaultsWidget;
	QTabWidget *twOptions;
};

#endif
