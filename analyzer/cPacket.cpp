#include <QString>
#include "cPacket.h"

void cPacket::parse(){
	memcpy_s(&MACheader, 14, data.data(), 14);
	//MACheader.type = qFromBigEndian(MACheader.type);
	//MACheader.set(data);
}

cPacket::operator QString(){
	return MACheader;
	return data.toHex();
}