#ifndef _CPACKET_H_
#define _CPACKET_H_

#include <QByteArray>
#include "IPheader.h"

class cPacket{
private:
	QByteArray data;
		IPheader header;

public:
	cPacket(QByteArray src);
	void parse();
	operator QString();
};
#endif

