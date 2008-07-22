#ifndef _CONNECTION_H_
#define _CONNECTION_H_
#include <QByteArray>
#include <QHostAddress>
#include <QTimer>
#include <QString>
#include <QCache>
#include <QPointer>
#include "ARecognizerEngine.h"
#include "Packet.h"
#include "IConnection.h"

#define DEFAULT_MAX 50

enum ConnectionField{
	Cf_Addr,
	Cf_PacketCount,
	Cf_Speed,
	Cf_Comment,
	Cf_All
};

class RManager;
class Connection:public IConnection {

	Q_OBJECT;

	QCache<QHostAddress, QString> * dns;
	const NetworkInfo info;
	QString nameSrc;
	QString nameDest;
	QString srvSrc;
	QString srvDest;
	QString shortDescFw;
	QString shortDescBc;
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
	const ARecognizerEngine* lastRec;
	
	Connection(const Connection& connection);
	const Connection& operator=(const Connection& other);

	void timerEvent(QTimerEvent * event);
	QString getSpeed(int speed) const;
	void countSpeed();

public slots:
//	void addPacket(const Packet& packet);
		void purge();
		void setAutoPurge(bool on);
		void setQuick(QPair<QString, QString> comm);
		void setLast(const ARecognizerEngine * engine) { lastRec = engine; };
		inline const ARecognizerEngine * getLast() const { return lastRec; };

signals:
	void changed(IConnection * me);
	void timedOut(IConnection * me);

public:
	~Connection();
	Connection(QCache<QHostAddress, QString>* dns_, bool death, const Packet& packet);
	Connection & operator<<(const Packet& packet);
	const QString toString() const;

	const NetworkInfo& networkInfo() const { return info; };
	const QByteArray getDataForw() const;
	const QByteArray getDataBack() const;
	const QByteArray getLastPacketFw() const 
				{ return dataForw.isEmpty()?QByteArray():dataForw.last(); };
	const QByteArray getLastPacketBc() const 
				{ return dataBack.isEmpty()?QByteArray():dataBack.last(); };
	inline int speedFw() const { return speedUp; };	
	inline int speedBc() const { return speedDown; };
	inline const QString sourceName() const { return nameSrc; };
	inline const QString destinationName() const { return nameDest; };
	inline int packetCountFw() const { return countFr; };
	inline int packetCountBc() const { return countBc; };
	inline const QString fwDesc() const { return shortDescFw; };
	inline const QString bcDesc() const { return shortDescBc; };

signals:
	void timedOut(Connection * me);
	void changed(Connection * me, ConnectionField);
	

};
#endif
