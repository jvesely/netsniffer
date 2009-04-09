#pragma once

#include "Packet.h"
#include "IConnection.h"

#define DEFAULT_MAX 50

enum ConnectionField
{
	Cf_Nothing = 0,
	Cf_Address = 1,
	Cf_PacketCount = 2,
	Cf_Speed = 4,
	Cf_Comment = 8,
	Cf_Status = 16,
	Cf_All = 31
};

//Q_DECLARE_METATYPE(ConnectionField);

typedef QPair<bool, QByteArray> DirectedData;
typedef QList<DirectedData> DirectedDataList;

//class RManager;

class Connection:public IConnection
{
public slots:
//	void addPacket(const Packet& packet);
//		void purge();
	void setAutoPurge( bool on );
	void setQuick( QPair<QString, QString> comm );
	void close();
	void die();
	void update( const QCache<QHostAddress, QString>* dns = NULL );

signals:
	void changed( Connection* me, uint part );

public:
	~Connection();

	Connection( const Packet& packet );
	Connection & operator << ( const Packet& packet );

	inline const NetworkInfo& networkInfo() const 
		{ QReadLocker lock(&m_guard); return m_info; };
//	const QByteArray getDataForw() const;
//	const QByteArray getDataBack() const;

	inline ConnStatus getStatus() const
		{ QReadLocker lock(&m_guard); return m_status; };

	inline const QByteArray getLastPacketFw() const 
		{ QReadLocker lock(&m_guard); return m_lastPacketForward; };

	inline const QByteArray getLastPacketBc() const 
		{ QReadLocker lock(&m_guard); return m_lastPacketBack; };

	inline int speedFw() const
		{ QReadLocker lock(&m_guard); return m_speedUp; };	

	inline int speedBc() const
		{ QReadLocker lock(&m_guard); return m_speedDown; };

	inline const QString sourceName() const 
		{ QReadLocker lock(&m_guard); return m_sourceName; };

	inline const QString destinationName() const
		{ QReadLocker lock(&m_guard); return m_destinationName; };
	
	inline int packetCountFw() const
		{ QReadLocker lock(&m_guard); return m_countForward; };

	inline int packetCountBc() const
		{ QReadLocker lock(&m_guard); return m_countBack; };
//	inline const QString fwDesc() const { return shortDescFw; };
//	inline const QString bcDesc() const { return shortDescBc; };


private:

	const NetworkInfo m_info;
	QString m_sourceName;
	QString m_destinationName;
	DirectedDataList m_data;
	QByteArray m_lastPacketForward;
	QByteArray m_lastPacketBack;
	uint m_countForward;
	uint m_countBack;
	int m_timeout;
	bool m_killDead;
	int m_speedUp;
	int	m_speedDown;
	int m_dataUp;
	int m_dataDown;
	
	ConnStatus m_status;
	QTimer m_deathTimer;

	mutable QReadWriteLock m_guard;

	Q_DISABLE_COPY (Connection);
	Q_OBJECT;
};
