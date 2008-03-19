#include <QString>
#include <QByteArray>
#include <QHash>
#include "CPacket.h"

/*----------------------------------------------------------------------------*/
CPacket::CPacket(QByteArray src) {
	ipHeader.parse(src);
	switch (ipHeader.trProtocol()) {
		case UDP:
			trHeader.udpHeader.parse(src.mid(ipHeader.headerLength(), UDPLength));
			data = src.mid(ipHeader.headerLength() + UDPLength);
			break;
		case TCP:
			break;
		default:
		// exception here
		 return;
	}
}
/*----------------------------------------------------------------------------*/
CPacket::operator QString() const {
	return (QString)ipHeader + 
		( (trProtocol() == UDP)?((QString)trHeader.udpHeader):((QString)trHeader.tcpHeader)) + "\nHASH:" + QString::number(hash());
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
		return trHeader.udpHeader.srcPort();
	return trHeader.tcpHeader.srcPort();
}
/*----------------------------------------------------------------------------*/
const quint16 CPacket::destPort() const {
	if ( trProtocol() == UDP )
		return trHeader.udpHeader.destPort();
	return trHeader.tcpHeader.destPort();//zatial
}
/*----------------------------------------------------------------------------*/
const TrProtocol CPacket::trProtocol() const {
	return ipHeader.trProtocol();
}
/*----------------------------------------------------------------------------*/
bool CPacket::operator==(const CPacket& packet) const {
	if (trProtocol() != packet.trProtocol())
		return false;
	if (trProtocol() == TCP)
		return hash() == packet.hash(); // they belong to the same sequence

	return (
		srcAddress() == packet.srcAddress() &&
		destAddress() == packet.destAddress() &&
		srcPort() == packet.srcPort() &&
		destPort() == packet.destPort()
	);
}
/*----------------------------------------------------------------------------*/
uint CPacket::hash() const {
	if (ipHeader.trProtocol() == TCP)
		return trHeader.tcpHeader.sequence();
	else
		return 	qHash(ipHeader.srcAddress().toIPv4Address()) ^ 
						qHash(ipHeader.destAddress().toIPv4Address()) ^
						qHash(trHeader.udpHeader.srcPort()) ^
						qHash(trHeader.udpHeader.destPort()); 

}
/*----------------------------------------------------------------------------*/
uint qHash(CPacket &packet) {
	return packet.hash();
}
