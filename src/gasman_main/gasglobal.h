
#ifndef GASGLOBAL_H
#define GASGLOBAL_H

#include <QStringList>
#include <math.h>
#include <QObject>

extern QString gasmanVersion;

const int MaxRecentFiles = 5;
const int MaxSimulations = 2;		/* Maximum number of simultaneous simulations */
const QStringList AvailableAgents = ( QStringList() << QObject::tr( "Isoflurane" ) << QObject::tr( "Nitrous Oxide" ) );	/* Available agents */

const QString FMT_FGF = "%4.2f";		/* How to format the numerics */
const QString FMT_VA = "%4.2f";
const QString FMT_CO = "%4.2f";

#define DT					6000	/* 6 thousand milliseconds = 6 sec */
#define MAX_SAMP			6000	/* maximum number of samples to retain */
#define SAMP_EVERY			10		/* ticks between samples */
#define MAX_HOURS			48		/* maximum experiment, depending on room for samples */
#define MAX_ADJ_PER_HOUR	60		/* max average adjustments per hour for an experiment */

#define AFAP			1000	/* 1000 times reality IS as fast as possible*/
#define MAX_BKMK		50		/* number of bookmarks */
#define MAX_VERNIER		5		/* chop it til we hit 1/2^4 = 1/16 */
#define VERNIER_TICKS	3		/* have vernier effective for 3 breaths */
#define STD_WEIGHT		70.0F	/* seventy KG */
#define STD_FGF			0.25F	/* 1/4 L/m */
#define STD_VA			4.0F	/* 4 L/m */
#define STD_CO			5.0F	/* 5 L/m */
#define MAX_WEIGHT		1500.0F	/* obeast */
#define MIN_WEIGHT		0.05F	/* 50 grams */
#define INIT_SEMI_FGF	8.0F	/* initial FGF if semi or ideal */
#define SEMI_FGF		3.0F	/* FGF if after time zero */
#define NGS				10		/* max simultaneous gasses */
#define MAX_FGF			500.0F	/* Highest allowable value */
#define MAX_VA			500.0F
#define MAX_CO			500.0F
#define MAX_BOTTLE_COST	10000.0F
#define MAX_BOTTLE_SIZE	5000
#define GSCALE			20


// The results we're after
enum AResult {
	CKT=0,
	ALV,
	VRG,
	MUS,
	FAT,
	VEN,
	MAX_COMPART,
	UPT=MAX_COMPART,
	DEL,
	MAX_RESULT
};

enum AState
{
	RUNNING_STATE,
	STOPPED_STATE,
	INITIAL_STATE,
	END_STATE
};

enum GasViewType
{
	PANEL=0,
	GRAPH,
	CONTROL
};

enum AnEvent
{
	UPD_NONE=0,
	UPD_AGENT=1,
	UPD_CIRCUIT=UPD_AGENT<<1,
	UPD_SPEED=UPD_CIRCUIT<<1,
	UPD_DEL=UPD_SPEED<<1,
	UPD_FGF=UPD_DEL<<1,
	UPD_VA=UPD_FGF<<1,
	UPD_CO=UPD_VA<<1,
	UPD_TICK=UPD_CO<<1,
	UPD_STATE=UPD_TICK<<1,
	UPD_WEIGHT=UPD_STATE<<1,
	UPD_INJECT=UPD_WEIGHT<<1,
	UPD_NGAS=UPD_INJECT<<1
};

//forward declaration
class GasChildWindow;
class GasDoc;
class GasControl;

typedef float COMP_ARRAY[MAX_COMPART];
typedef float RES_ARRAY[MAX_RESULT];
typedef QList<GasChildWindow *> GasChildren;
typedef QListIterator<GasChildWindow *> GasChildIterator;
typedef QList<GasControl *> GasControls;
typedef QMap<QString,QVariant> GasAttributeMap;
typedef int EventList;										// These are lists of interesting
															// events built by hint objects

// Global storage

inline bool FNear( float f1, float f2 )
{
	return fabs( f1-f2 ) * 10000 < fabs( f1 < f2 ? f1 : f2 );
}

extern void InitializeGlobalLists();
extern QString ToQColor(const QString &);
extern QString completeBaseName( const QString &fullFileName );
extern QString fileName( const QString &fullFileName );

extern QStringList listCompart;
extern QStringList listTargetCompart;
extern QStringList listCircuit;
extern QStringList listSpeed;
extern QStringList listTime;
extern QStringList listColors;
extern int linesCount;
extern QList<GasDoc *> docs;

#endif
