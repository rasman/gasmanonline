#include "gasparser.h"
#include <QDebug>
GasParser::GasParser(QObject* parent):
	QObject(parent)
{
	Op<<"FGF"<<"DEL"<<"CKT"<<"ALV"<<"MUS"<<"FAT"<<"VRG";
	Si<<"+"<<"-"<<"*"<<"/" ;
	//Com<<"<"<<">"<<"=";
	bop = false;
}


bool GasParser::parse(QString str)
{	
	OpStack.clear();
	SiStack.clear();

	str  = str.simplified();
	if(Si.contains(str.at(0), Qt::CaseInsensitive) )
			return false;
	 
	if(str.count("=") != 1)
			return false; 

	int a = str.indexOf("=");
	bool ok;
//	int b = str.length();
	result = str.right(str.length()-a-1).simplified().toFloat(&ok);
	
	if(!ok)
		return false;
	
	str  = str.left(a);

	if(str.length() ==0)
		return false;
	
	

	while(str.length()>0){
		 int i = 0;
		 if(str.at(i)==' '){
			 str = str.right(str.length()-1);
			 continue;
			}
		if(!Si.contains(str.at(i))){
			if(!bop){
			if(Op.contains(str.left(3))){
				OpStack.append(str.left(3));
				i+=2;
				 str = str.right(str.length()-3);
				bop = !bop;				
				 continue;
			}else{
			
				return false;
			}
				
			}else{
				 			
				return false;
			}
				
		}else{
			if(bop){
			SiStack.append(str.at(i));
			str = str.right(str.length()-1);
			bop = !bop;
			}else{
				 
				return false;
			}
		}
		 
	}
	if(SiStack.size() != OpStack.size() -1)
		return false;

	return true;

}