#ifndef _CONNECTION_H_
#define _CONNECTION_H_
#include <QByteArray>
#include <QHostAddress>
#include <QTimer>
#include <QString>
#include <QCache>
#include <QPointer>
#include "Packet.h"
#include "Recognizer.h"
#include "IConnection.h"

#define DEFAULT_MAX 50

class RManager;
class Connection:public IConnection {

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
	QList<QByteArray> dataForw;
	QList<QByteArray> dataBack;
	int maxFw;
	int maxBc;
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
	QPointer<ARecognizerEngine> lastRec;
	
	Connection(const Connection& connection);
	const Connection& operator=(const Connection& other);

	void reset(bool start = false);
	void timerEvent(QTimerEvent * event);
	QString getSpeed(int speed) const;
	void countSpeed();
public slots:
//	void addPacket(const Packet& packet);
		void purge();
		void setAutoPurge(bool on);
		void setQuick(QPair<QString, QString> comm);
		void setLast(const ARecognizerEngine * engine);
		const ARecognizerEngine * getLast() const;

signals:
	void changed(Connection * me);
	void timedOut(Connection * me);

public:
	Connection();
	~Connection();
	Connection(QCache<QHostAddress, QString>* dns_, bool death, RManager* recs);
	Connection & operator<<(const Packet& packet);
	void setCache(QCache<QHostAddress, QString>* cache);
	void setRecognizers(RManager * recognizer);
	int packetCount() const throw();
	operator QByteArray() const;
	const QString toString() const;

	const QByteArray getDataForw() const;
	const QByteArray getDataBack() const;
	const QByteArray getLastPacketFw() const;
	const QByteArray getLastPacketBc() const;
	const QHostAddress getAddrSrc() const;
	const QHostAddress getAddrDest() const;
	const quint16 getPortSrc() const;
	const quint16 getPortDest() const;
	const TrProtocol getProto() const;
	
	void setBcPacketCount(int count);
	void setFwPacketCount(int count);

};
#endif
