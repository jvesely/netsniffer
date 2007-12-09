#ifndef _CPACKET_H_
#define _CPACKET_H_
#include <QByteArray>
class cPacket{
private:
	QByteArray data;
public:
	cPacket(const char * data_, const int len):data(data_,len){};
	void write();
};

#endif