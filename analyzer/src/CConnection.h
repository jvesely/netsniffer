#ifndef _CCONNECTION_H_
#define _CCONNECTION_H_
#include <QByteArray>
#include <QHostAddress>
#include <QTimer>
#include <QString>
#include "CPacket.h"

class CConnection:public QObject {
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
//	void addPacket(const CPacket& packet);

signals:
	void addedPacket(CConnection * me);
	void timedOut(CConnection * me);

public:
	CConnection();
	CConnection(const CConnection& connection);
	CConnection & operator<<(const CPacket& packet);
	int packetCount() const throw();
	operator QByteArray() const;
	const QString toString() const;
};
#endif
