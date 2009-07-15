#pragma once

#include "Packet.h"
#include "IConnection.h"

typedef QQueue<IConnection::DirectedPacket> DirectedPacketQueue;

class Connection:public IConnection
{
public slots:
	bool setAutoPurge( bool on );
	void close();
	void die();
	virtual void update();

public:
	typedef QExplicitlySharedDataPointer<Connection> Pointer;

	Connection( const Packet& packet );
	~Connection();

	inline Connection& operator << ( const Packet& packet )
		{ return addPacket( packet ),*this; }

	inline const NetworkInfo& networkInfo() const 
		{ return m_info; };

	inline ConnectionStatus status() const
		{ QReadLocker lock( &m_guard ); return m_status; };

	inline const DirectedPacket nextPacket()
		{ QReadLocker lock( &m_guard ); Q_ASSERT(m_data.count()); return m_data.dequeue(); }
	
	inline const DirectedPacket topPacket() const
		{ QReadLocker lock( &m_guard ); Q_ASSERT(m_data.count()); return m_data.head(); }

	inline const DataCount countData() const
		{ return DataCount( m_dataUp, m_dataDown ); };

	const PacketCount waitingPackets() const;
	
	inline const PacketCount totalPackets() const
		{ return PacketCount( m_countForward, m_countBack ); };

	inline const Speed speed() const
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

