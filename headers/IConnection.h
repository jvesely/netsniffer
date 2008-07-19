#ifndef _ICONNECTION_H_
#define _ICONNECTION_H_

#include <QByteArray>
#include <QObject>
#include <QHostAddress>
#include <QObject>

#include "protocol.h"

class IConnection:public QObject {
	Q_OBJECT;
public:
	virtual ~IConnection() {};
	virtual const QByteArray getLastPacketFw() const = 0;
	virtual	const QByteArray getLastPacketBc() const = 0;
	virtual const QByteArray getDataForw() const = 0;
	virtual const QByteArray getDataBack() const = 0;
	virtual const QHostAddress getAddrSrc() const = 0;
	virtual const QHostAddress getAddrDest() const = 0;
	virtual const quint16 getPortSrc() const = 0;
	virtual const quint16 getPortDest() const = 0;
	virtual const TrProtocol getProto() const = 0;
	virtual const QString toString() const = 0;

public slots:
	virtual void setFwPacketCount(int count) = 0;
	virtual	void setBcPacketCount(int count) = 0;

signals:
	void timedOut(IConnection *);

};
#endif
