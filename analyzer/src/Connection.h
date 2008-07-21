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
		void setLast(const ARecognizerEngine * engine);
		const ARecognizerEngine * getLast() const;

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
	inline int packetCountFw() const { return dataUp; };
	inline int packetCountBc() const { return dataDown; };
	inline const QString fwDesc() const { return shortDescFw; };
	inline const QString bcDesc() const { return shortDescBc; };

signals:
	void timedOut(Connection * me);
	void changed(Connection * me);
	

};
#endif
