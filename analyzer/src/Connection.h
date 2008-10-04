#pragma once

#include "ARecognizerEngine.h"
#include "Packet.h"
#include "IConnection.h"

#define DEFAULT_MAX 50

enum ConnectionField{
	Cf_Address,
	Cf_PacketCount,
	Cf_Speed,
	Cf_Comment,
	Cf_Status,
	Cf_All
};

Q_DECLARE_METATYPE(ConnectionField);

class RManager;
class Connection:public IConnection {

	Q_OBJECT;

	const QCache<QHostAddress, QString> & dns;
	const NetworkInfo info;
	QString nameSrc;
	QString nameDest;
	QString shortDescFw;
	QString shortDescBc;
	QList<QPair<bool, QByteArray> > data;
	QByteArray lastPacketForward;
	QByteArray lastPacketBack;
	int deathTimer;
	int speedTimer;
	int timeout;
	uint countFr;
	uint countBc;
	ConnStatus status;
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
//		void purge();
		void setAutoPurge(bool on);
		void setQuick(QPair<QString, QString> comm);
		inline void setLast(const ARecognizerEngine * engine) { lastRec = engine; };
		inline const ARecognizerEngine * getLast() const { return lastRec; };
	void close();

signals:
	void changed(IConnection * me);
	void timedOut(IConnection * me);

public:
	~Connection();

		Connection(const QCache<QHostAddress, QString> & dns_, bool death, const Packet& packet);
	Connection & operator<<(const Packet& packet);
	//const QString toString() const;


	inline const NetworkInfo& networkInfo() const { return info; };
//	const QByteArray getDataForw() const;
//	const QByteArray getDataBack() const;
	inline const ConnStatus getStatus() const
				{ return status; };
	inline const QByteArray getLastPacketFw() const 
				{ return lastPacketForward; };
	inline const QByteArray getLastPacketBc() const 
				{ return lastPacketBack; };
	inline int speedFw() const { return speedUp; };	
	inline int speedBc() const { return speedDown; };
	inline const QString sourceName() const { return nameSrc; };
	inline const QString destinationName() const { return nameDest; };
	inline int packetCountFw() const { return countFr; };
	inline int packetCountBc() const { return countBc; };
	inline const QString fwDesc() const { return shortDescFw; };
	inline const QString bcDesc() const { return shortDescBc; };

signals:
	//void timedOut(Connection * me);
	void changed(Connection * me, ConnectionField part);
	

};
