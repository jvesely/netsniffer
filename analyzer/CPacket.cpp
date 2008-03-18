#include <QString>
#include <QByteArray>
#include "cPacket.h"

cPacket::cPacket(QByteArray src) {
	data = src;
	header.parse(src);
}
void cPacket::parse(){
}

cPacket::operator QString(){
	return header;
	return data.toHex();
}
