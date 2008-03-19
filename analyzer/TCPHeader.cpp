#include <QtCore>
#include "TCPHeader.h"
#define TCPHeaderLengthMIN 20
#define HALF_BYTE 16

/*---------------------------------------------------------------------------*/
void TCPHeader::parse(QByteArray src) {
	if (src.length() < TCPHeaderLengthMIN )
		return;
	const char * data = src.data();

	srcport = qFromBigEndian(*(quint16*)(data));
	destport = qFromBigEndian(*(quint16*)(data + 2));
	seq = qFromBigEndian(*(quint32*)(data + 4));
	acknum = qFromBigEndian(*(quint32*)(data + 8));
	len = (*(quint8*)(data + 12));
	len /= HALF_BYTE; // consider just first 4 bits
}
/*---------------------------------------------------------------------------*/
const quint16 TCPHeader::srcPort() const throw() {
	return srcport;
}
/*---------------------------------------------------------------------------*/
const quint16 TCPHeader::destPort() const throw() {
	return destport;
}
/*---------------------------------------------------------------------------*/
const quint32 TCPHeader::sequence() const throw() {
	return seq;
}
/*---------------------------------------------------------------------------*/
const quint8 TCPHeader::length() const throw() {
	return len;
}
/*---------------------------------------------------------------------------*/
TCPHeader::operator QString() const {
	QString ret("\nTCP Header:");
	ret.append("\nSource Port: " + QString::number(srcport));
	ret.append("\nDestination Port: " + QString::number(destport));
	ret.append("\nLength: " + QString::number(len));
	ret.append("\nSeq : " + QString::number(seq));
	return ret;
}
