#include <QString>
#include "cPacket.h"

void cPacket::parse(){
	memcpy(&MACheader, data.data(), 14);
	//MACheader.type = qFromBigEndian(MACheader.type);
	//MACheader.set(data);
}

cPacket::operator QString(){
	return MACheader;
	return data.toHex();
}
