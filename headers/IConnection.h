#pragma once

#include "NetworkInfo.h"

class IConnection
{
public:
	enum ConnectionStatus {	Alive, Dead, TimedOut, Closed	};
	enum Direction { Forward, Back };
	typedef QPair<Direction, QByteArray> DirectedPacket;

	virtual ~IConnection() {};
	virtual void close() = 0;
	virtual const NetworkInfo& networkInfo() const = 0;
	virtual const DirectedPacket nextPacket() = 0;
	virtual const DirectedPacket topPacket() const = 0;
	virtual uint waitingPackets() = 0;

//	virtual const QByteArray getLastPacketForward() const = 0;
//	virtual	const QByteArray getLastPacketBack() const = 0;
	virtual int packetCountForward() const = 0;
	virtual int packetCountBack() const = 0;
	virtual ConnectionStatus status() const = 0;
};
