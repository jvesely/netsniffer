#pragma once

#include "NetworkInfo.h"
#include "IPHeader.h"
#include "TCPHeader.h"
#include "UDPHeader.h"

class Packet{
private:
	QByteArray data;
	IPHeader ipHeader;
	union {
		TCPHeader tcp;
		UDPHeader udp;
	} header;

public:
	Packet(QByteArray src);
	const NetworkInfo networkInfo() const;
	const QHostAddress srcAddress() const;
	const QHostAddress destAddress() const;
	const quint16 srcPort() const;
	const quint16 destPort() const;
	const TrProtocol trProtocol() const;
	operator QString() const;
	bool operator==(const Packet & packet) const;
	uint hash() const;
	const QByteArray getData() const;
//	operator QByteArray() const;
};

uint qHash(const Packet &packet);
