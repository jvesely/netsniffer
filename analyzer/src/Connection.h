#ifndef _CCONNECTION_H_
#define _CCONNECTION_H_
#include <QByteArray>
#include <QHostAddress>
#include <QTimer>
#include <QString>
#include "Packet.h"

class Connection:public QObject {
	Q_OBJECT;
	QHostAddress addrSrc;
	QHostAddress addrDest;
	quint16 portSrc;
	quint16 portDest;
	TrProtocol protocol;
	QByteArray dataForw;
	QByteArray dataBack;
	int timer;
	int timeout;
	uint countFr;
	uint countBc;

	void reset();
	void timerEvent(QTimerEvent * event);
public slots:
//	void addPacket(const Packet& packet);

signals:
	void addedPacket(Connection * me);
	void timedOut(Connection * me);

public:
	Connection();
	Connection(const Connection& connection);
	Connection & operator<<(const Packet& packet);
	int packetCount() const throw();
	operator QByteArray() const;
	const QString toString() const;
};
#endif
