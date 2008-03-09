#ifndef _CPACKET_H_
#define _CPACKET_H_

#include <QByteArray>
#include "cMACheader.h"

class cPacket{
private:
	QByteArray data;
	cMACheader MACheader;

public:
	cPacket(const char * data_, const int len):data(data_,len){};
	cPacket(QByteArray src):data(src){};
	void parse();
	operator QString();
};
#endif

