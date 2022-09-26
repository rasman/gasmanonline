#ifndef GASPARSER_H_
#define GASPARSER_H_
#include <QObject>
#include <QStack>
#include <QStringList>
class GasParser: public QObject {
	Q_OBJECT
public:
	QStringList Op;
	QStringList Si;
 
	QStringList OpStack;
	QStringList SiStack;
	float result;
	bool bop;
public :
	GasParser(QObject * parent = 0);
	bool parse(QString str);	
};


#endif