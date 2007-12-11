#ifndef _CMACHEADER_H_
#define _CMACHEADER_H_

#include <QString>
#include <QHash>

class cMACheader{	
public:

	char destaddr[6];
	char srcaddr[6];
	char type[2];

	operator QString();
private:
	
};
#endif