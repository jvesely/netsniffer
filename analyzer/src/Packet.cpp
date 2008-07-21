#include <QString>
#include <QByteArray>
#include <QHash>
#include "Packet.h"

/*----------------------------------------------------------------------------*/
Packet::Packet(QByteArray src) {
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
Packet::operator QString() const {
	return (QString)ipHeader + 
		( (trProtocol() == UDP)?((QString)header.udp):((QString)header.tcp)) + "\nHASH: " + QString::number(hash());
	return data.toHex();
}
/*----------------------------------------------------------------------------*/
const QHostAddress Packet::srcAddress() const {
	return ipHeader.srcAddress();
}
/*----------------------------------------------------------------------------*/
const QHostAddress Packet::destAddress() const {
	return ipHeader.destAddress();
}
/*----------------------------------------------------------------------------*/
const quint16 Packet::srcPort() const {
	if ( trProtocol() == UDP )
		return header.udp.srcPort();
	return header.tcp.srcPort();
}
/*----------------------------------------------------------------------------*/
const quint16 Packet::destPort() const {
	if ( trProtocol() == UDP )
		return header.udp.destPort();
	return header.tcp.destPort();//zatial
}
/*----------------------------------------------------------------------------*/
const TrProtocol Packet::trProtocol() const {
	return ipHeader.trProtocol();
}
/*----------------------------------------------------------------------------*/
bool Packet::operator==(const Packet& packet) const {

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
uint Packet::hash() const {
	return 	qHash(srcAddress().toIPv4Address()) ^ 
					qHash(destAddress().toIPv4Address()) ^
					qHash(srcPort()) ^
					qHash(destPort()) ^
					qHash((int)trProtocol()); 

}
/*----------------------------------------------------------------------------*/
const QByteArray Packet::getData() const {
	return data;
}
/*----------------------------------------------------------------------------*/
const NetworkInfo Packet::networkInfo() const {
	NetworkInfo info;
	info.sourceIP = ipHeader.srcAddress();
	info.destinationIP = ipHeader.destAddress();
	info.protocol = ipHeader.trProtocol();
	info.sourcePort = srcPort();
	info.destinationPort = destPort();
	return info;
}
/*----------------------------------------------------------------------------*/
uint qHash(const Packet &packet) {
	return packet.hash();
}
/*----------------------------------------------------------------------------*/




