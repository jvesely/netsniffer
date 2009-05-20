#pragma once

#include "Packet.h"
#include "IConnection.h"

typedef QQueue<IConnection::DirectedPacket> DirectedPacketQueue;

class Connection;

typedef QExplicitlySharedDataPointer<Connection> ConnectionPtr;

class Connection:public QObject, public IConnection, public QSharedData
{
public slots:
	void setAutoPurge( bool on );
	void close();
	void die();
	virtual void update();

signals:
	void packetArrived( ConnectionPtr me );
	void statusChanged( ConnectionPtr me );
	void finished( ConnectionPtr me );

public:
	~Connection();

	Connection( const Packet& packet );

	inline Connection& operator << ( const Packet& packet )
		{ return addPacket( packet ),*this; }

	inline const NetworkInfo& networkInfo() const 
		{ return m_info; };

	inline ConnectionStatus status() const
		{ QReadLocker lock(&m_guard); return m_status; };

	inline const DirectedPacket nextPacket()
		{ QReadLocker lock(&m_guard); return m_data.dequeue(); }

	inline uint waitingPackets()
		{ QReadLocker lock(&m_guard); return m_data.count(); }

	inline int speedForward() const
		{ QReadLocker lock(&m_guard); return m_speedUp; };	

	inline int speedBack() const
		{ QReadLocker lock(&m_guard); return m_speedDown; };

	inline int packetCountForward() const
		{ QReadLocker lock(&m_guard); return m_countForward; };

	inline int packetCountBack() const
		{ QReadLocker lock(&m_guard); return m_countBack; };

	virtual bool addPacket( const Packet& packet );

	static const int MAX_PACKETS_IN_QUEUE = 50;

protected:
	uint m_timeout;
	ConnectionStatus m_status;

private:
	const NetworkInfo m_info;
	
	DirectedPacketQueue m_data;
	
	uint m_countForward;
	uint m_countBack;
	bool m_killDead;
	int m_speedUp;
	int	m_speedDown;
	int m_dataUp;
	int m_dataDown;

	mutable QReadWriteLock m_guard;

	Q_DISABLE_COPY (Connection);
	Q_OBJECT;
};

