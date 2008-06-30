#include <QtCore>
#include "UDPHeader.h"

/*----------------------------------------------------------------------------*/
void UDPHeader::parse(QByteArray src) {
	if (src.length() < UDPLength)
		return;
	const char* data = src.data();
	srcport =	qFromBigEndian(*(quint16*)(data));
	destport = qFromBigEndian(*(quint16*)(data + 2));
	len = 	qFromBigEndian(*(quint16*)(data + 4));
	checksum = qFromBigEndian(*(quint16*)(data + 6));

}
/*----------------------------------------------------------------------------*/
const quint16 UDPHeader::srcPort() const throw() {
	return srcport;
}
/*----------------------------------------------------------------------------*/
const quint16 UDPHeader::destPort() const throw() {
	return destport;
}
/*----------------------------------------------------------------------------*/
const quint16 UDPHeader::length() const throw() {
	return len;
}
/*----------------------------------------------------------------------------*/
UDPHeader::operator QString() const {
	QString ret("\nUDP header:");
	ret.append("\nSourcePort: " + QString::number(srcport));
	ret.append("\nDestPort: "  + QString::number(destport));
	ret.append("\nLength: "  + QString::number(len));
	return ret;

}
