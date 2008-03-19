#ifndef _CPACKET_H_
#define _CPACKET_H_

#include <QByteArray>
#include "IPHeader.h"
#include "TCPHeader.h"
#include "UDPHeader.h"

class CPacket{
private:
	QByteArray data;
	IPHeader ipHeader;
	union {
		TCPHeader tcpHeader;
		UDPHeader udpHeader;
	} trHeader;

public:
	CPacket(QByteArray src);
	const QHostAddress srcAddress() const;
	const QHostAddress destAddress() const;
	const quint16 srcPort() const;
	const quint16 destPort() const;
	const TrProtocol trProtocol() const;
	operator QString() const;
//	operator QByteArray() const;
};
#endif

