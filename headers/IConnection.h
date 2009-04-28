#pragma once

#include "NetworkInfo.h"

class IConnection:public QObject
{
public:
	enum ConnectionStatus {	Alive, Dead, TimedOut, Closed	};

	virtual ~IConnection() {};
	virtual void close() = 0;
	virtual const NetworkInfo& networkInfo() const = 0;
	virtual const QByteArray getLastPacketForward() const = 0;
	virtual	const QByteArray getLastPacketBack() const = 0;
//	virtual const QByteArray getDataForw() const = 0;
//	virtual const QByteArray getDataBack() const = 0;
	virtual int packetCountFw() const = 0;
	virtual int packetCountBc() const = 0;
	virtual ConnectionStatus getStatus() const = 0;
	//virtual int speedFw() const = 0;
	//virtual int speedBc() const = 0;
};
