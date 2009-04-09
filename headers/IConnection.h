#pragma once

#include "NetworkInfo.h"

//class ARecognizerEngine;
class IConnection:public QObject
{
public:
	enum ConnStatus{
		Cs_Alive = 0, Cs_Dead = 1, Cs_TimedOut = 2, Cs_Closed = 3
	};

	virtual ~IConnection() {};
	virtual void close() = 0;
	virtual const NetworkInfo& networkInfo() const = 0;
	virtual const QByteArray getLastPacketFw() const = 0;
	virtual	const QByteArray getLastPacketBc() const = 0;
//	virtual const QByteArray getDataForw() const = 0;
//	virtual const QByteArray getDataBack() const = 0;
	virtual int packetCountFw() const = 0;
	virtual int packetCountBc() const = 0;
	virtual ConnStatus getStatus() const = 0;
	//virtual int speedFw() const = 0;
	//virtual int speedBc() const = 0;
	//virtual const QString commentFw() const = 0;
	//virtual const QString commentBc() const = 0;

//	virtual const ARecognizerEngine * getLast() const = 0;
	//virtual const QString toString() const = 0;
};
