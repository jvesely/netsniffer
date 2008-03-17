#include "IPheader.h"
#include <QtCore>

#define HALF_BYTE 16
#define FRAGMENT 0x4000 //2nd bit in 16bit number (big endian)
#define MORE_FRAGS 0x2000 //3rd bit in 16 bit n umber (big endian)
#define UDP 17
#define TCP 6

void IPheader::parse(QByteArray src) {
	char * data = src.data();
	quint8 tmp = *data;
	
	version = tmp / HALF_BYTE;
	length = (tmp % HALF_BYTE) * 4;
	//qDebug() << "tmp:" << tmp << "Version: " << version << "Length" << length <<endl;
	type = *(++data);
	totlen = qFromBigEndian(*(quint16 *)(++data));
	//qDebug() << "ToS: " << type << "PacketLength: " << totlen << endl;
	++data; //two bytes for total length;
	id = *(++data);
	++data; //two bytes for identificator;
	froffset = qFromBigEndian(*(quint16*)(++data));
	fragment = (froffset & FRAGMENT); //2nd bit in 2 bytes
	morefrags = froffset & MORE_FRAGS; //3rd bit in 2 bytes

	//remove first those bits
	froffset %= MORE_FRAGS;
	//qDebug() << "Offset: " << froffset << "Fragment? : " << fragment << "morefrags?: " << morefrags;
	++data; // two bytes for tags and offset counter
	ttl = *(++data);
	protocol = *(++data);

	checksum = qFromBigEndian(*(quint16*)(++data));
	//qDebug() << "TTL: " << ttl << "Protocol: " << protocol << "Checksum: " << checksum;
	++data; //two bytes for checksum
	source.setAddress(qFromBigEndian(*(quint32*)(++data)));
	data += 4;
	destination.setAddress(qFromBigEndian(*(quint32*)(data)));
	//compulsory part ended;
	bin = src.left(length);
}

IPheader::operator QString() {
//	return bin.toHex();
	QString ret("Header:\n");
	ret.append("IP Version: " + QString::number(version));
	ret.append("\nHeader length: " + QString::number(length));
	ret.append("\nToS" + type);
	ret.append("\nPacket Length: " + QString::number(totlen));
	ret.append("\nIdentification: " + QString::number(id, 16));
	ret.append("\nFragment: " + fragment);
	ret.append("\nMore? :" + morefrags);
	ret.append("\nOffset: " + QString::number(froffset));
	ret.append("\nTTL: " + QString::number(ttl));
	ret.append("\nProtocol: ");
	switch (protocol){
		case UDP:
			ret.append("UDP");
			break;
		case TCP:
			ret.append("TCP");
			break;
		default:
			ret.append("Other");
	}
	ret.append("\nChecksum: " + QString::number(checksum, 16));
	ret.append("\nFrom: " + source.toString());
	ret.append("\nTo: " + destination.toString());
	ret.append("\n" + bin.toHex());
	return ret;
}
