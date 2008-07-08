#ifndef _CONNECTION_H_
#define _CONNECTION_H_
#include <QByteArray>
#include <QHostAddress>
#include <QTimer>
#include <QString>
#include <QCache>
#include "Packet.h"
#include "RManager.h"

class Connection:public QObject {
	Q_OBJECT;
	QCache<QHostAddress, QString> * dns;
	RManager * recognizers;
	QHostAddress addrSrc;
	QHostAddress addrDest;
	QString nameSrc;
	QString nameDest;
	quint16 portSrc;
	quint16 portDest;
	QString srvSrc;
	QString srvDest;
	QString shortDescFw;
	QString shortDescBc;
	TrProtocol protocol;
	QByteArray dataForw;
	QByteArray dataBack;
	int deathTimer;
	int speedTimer;
	int timeout;
	uint countFr;
	uint countBc;
	bool dead;
	bool killDead;
	int speedUp;
	int	speedDown;
	int dataUp;
	int dataDown;
	int lastRec;

	void reset();
	void timerEvent(QTimerEvent * event);
	QString getSpeed(int speed) const;
	void countSpeed();
public slots:
//	void addPacket(const Packet& packet);
		void purge();
		void setAutoPurge(bool on);

signals:
	void addedPacket(Connection * me);
	void timedOut(Connection * me);

public:
	Connection();
	Connection(QCache<QHostAddress, QString>* dns_, bool death, RManager* recs);
	Connection(const Connection& connection);
	Connection & operator<<(const Packet& packet);
	void setCache(QCache<QHostAddress, QString>* cache);
	void setRecognizers(RManager * recognizer);
	int packetCount() const throw();
	operator QByteArray() const;
	const QString toString() const;

};
#endif
