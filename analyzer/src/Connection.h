#pragma once

#include "Packet.h"
#include "IConnection.h"

typedef QQueue<IConnection::DirectedPacket> DirectedPacketQueue;

class Connection:public IConnection
{
public slots:
	void setAutoPurge( bool on );
	void close();
	void die();
	virtual void update();

public:
	typedef QExplicitlySharedDataPointer<Connection> Pointer;
	~Connection();

	Connection( const Packet& packet );

	inline Connection& operator << ( const Packet& packet )
		{ return addPacket( packet ),*this; }

	const NetworkInfo& networkInfo() const 
		{ return m_info; };

	ConnectionStatus status() const
		{ QReadLocker lock( &m_guard ); return m_status; };

	const DirectedPacket nextPacket()
		{ QReadLocker lock( &m_guard ); Q_ASSERT(m_data.count()); return m_data.dequeue(); }
	
	const DirectedPacket topPacket() const
		{ QReadLocker lock( &m_guard ); Q_ASSERT(m_data.count()); return m_data.head(); }

	const DataCount countData() const
		{ return DataCount( m_dataUp, m_dataDown ); };

	const PacketCount waitingPackets() const;
	
	const PacketCount totalPackets() const
		{ return PacketCount( m_countForward, m_countBack ); };

	const Speed speed() const
		{ return Speed( m_speedUp, m_speedDown ); };	

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
	quint64 m_dataUp;
	quint64 m_dataDown;

	mutable QReadWriteLock m_guard;

	Q_DISABLE_COPY (Connection);
	Q_OBJECT;
};

