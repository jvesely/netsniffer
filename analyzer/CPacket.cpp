#include <QString>
#include <QByteArray>
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
		( (trProtocol() == UDP)?((QString)trHeader.udpHeader):((QString)trHeader.tcpHeader));
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
	//zatial
	return 0;
}
/*----------------------------------------------------------------------------*/
const quint16 CPacket::destPort() const {
	//zatial
	return 0;
}
/*----------------------------------------------------------------------------*/
const TrProtocol CPacket::trProtocol() const {
	return ipHeader.trProtocol();
}
