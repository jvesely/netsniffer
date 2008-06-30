#include <QString>
#include <QByteArray>
#include <QHash>
#include "CPacket.h"

/*----------------------------------------------------------------------------*/
CPacket::CPacket(QByteArray src) {
	ipHeader.parse(src);
	switch (ipHeader.trProtocol()) {
		case UDP:
			header.udp.parse(src.mid(ipHeader.headerLength(), UDPLength));
			data = src.mid(ipHeader.headerLength() + UDPLength);
			break;
		case TCP:
			header.tcp.parse(src.mid(ipHeader.headerLength()));
			data = src.mid(ipHeader.headerLength() + header.tcp.length());
			break;
		default:
		// exception here
		 return;
	}
}
/*----------------------------------------------------------------------------*/
CPacket::operator QString() const {
	return (QString)ipHeader + 
		( (trProtocol() == UDP)?((QString)header.udp):((QString)header.tcp)) + "\nHASH: " + QString::number(hash());
	return data.toHex();
}
/*----------------------------------------------------------------------------*/
const QHostAddress CPacket::srcAddress() const {
	return ipHeader.srcAddress();
}
/*----------------------------------------------------------------------------*/
const QHostAddress CPacket::destAddress() const {
	return ipHeader.destAddress();
}
/*----------------------------------------------------------------------------*/
const quint16 CPacket::srcPort() const {
	if ( trProtocol() == UDP )
		return header.udp.srcPort();
	return header.tcp.srcPort();
}
/*----------------------------------------------------------------------------*/
const quint16 CPacket::destPort() const {
	if ( trProtocol() == UDP )
		return header.udp.destPort();
	return header.tcp.destPort();//zatial
}
/*----------------------------------------------------------------------------*/
const TrProtocol CPacket::trProtocol() const {
	return ipHeader.trProtocol();
}
/*----------------------------------------------------------------------------*/
bool CPacket::operator==(const CPacket& packet) const {

	bool ret =  (
		trProtocol() == packet.trProtocol() &&			//protocol matches
		( (	
				(srcAddress() == packet.srcAddress()) && // forward direction
				(destAddress() == packet.destAddress()) &&
				(srcPort() == packet.srcPort()) &&
				(destPort() == packet.destPort())
			) || (			//return trip
				(srcAddress() == packet.destAddress()) &&
				(destAddress() == packet.srcAddress()) &&
				(srcPort() == packet.destPort()) &&
				(destPort() == packet.srcPort())
		))
	);
	return ret;
}
/*----------------------------------------------------------------------------*/
uint CPacket::hash() const {
	return 	qHash(srcAddress().toIPv4Address()) ^ 
					qHash(destAddress().toIPv4Address()) ^
					qHash(srcPort()) ^
					qHash(destPort()) ^
					qHash((int)trProtocol()); 

}
/*----------------------------------------------------------------------------*/
const QByteArray CPacket::getData() const {
	return data;
}
/*----------------------------------------------------------------------------*/
uint qHash(const CPacket &packet) {
	return packet.hash();
}

