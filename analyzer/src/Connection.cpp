#include "Connection.h"

#define DEBUG_TEXT "[ Connection ]:"
#include "debug.h"

#define TIMEOUT_INTERVAL 5

#define MY_WAY(PACKET) \
	(m_info.protocol == PACKET.protocol &&\
	m_info.sourceIP == PACKET.sourceIP &&\
	m_info.destinationIP == PACKET.destinationIP &&\
	m_info.sourcePort == PACKET.sourcePort &&\
	m_info.destinationPort == PACKET.destinationPort )

#define BACK_WAY(PACKET) \
	(m_info.protocol == PACKET.protocol &&\
	m_info.sourceIP == PACKET.destinationIP &&\
	m_info.destinationIP == PACKET.sourceIP &&\
	m_info.sourcePort == PACKET.destinationPort &&\
	m_info.destinationPort == PACKET.sourcePort )


Connection::Connection( const Packet& packet ):
	m_timeout( TIMEOUT_INTERVAL ), m_status( Alive ),
	m_info( packet.networkInfo() ),	m_countForward( 1 ), m_countBack( 0 ),
	m_speedDown( 0 ), m_dataDown( 0 )	
{
	m_data.enqueue( DirectedPacket( Forward, packet.data() ) );
	m_speedUp = m_dataUp = packet.data().size();
	PRINT_DEBUG ("-------------Creating" << this << "-----------");
}
/*----------------------------------------------------------------------------*/
Connection::~Connection()
{
	QWriteLocker locker( &m_guard ); // wait if something is by any chance inserting packet
	PRINT_DEBUG ("----------------Dying" << this << "--------------");
}
/*----------------------------------------------------------------------------*/
void Connection::update()
{
		QWriteLocker lock( &m_guard );
		m_speedUp = m_speedDown = 0;
}
/*----------------------------------------------------------------------------*/
void Connection::close()
{
	m_status = Closed;
	QTimer::singleShot( TIMEOUT_INTERVAL * 1000, this, SLOT(die()) );
	emit statusChanged( IConnection::Pointer( this ) );
	PRINT_DEBUG ("Closed connection.." << this);
}
/*----------------------------------------------------------------------------*/
void Connection::die()
{
	if (m_status != Closed)
		return;

	PRINT_DEBUG ("Connection dying" <<  this);
	m_status = Dead;

	if (m_killDead) {
		emit finished( IConnection::Pointer( this ) );
	} else {
		emit statusChanged( IConnection::Pointer( this ) );
	}
}
/*----------------------------------------------------------------------------*/
void Connection::setAutoPurge( bool on )
{
	{
		QWriteLocker lock( &m_guard );
		m_killDead = on;
		PRINT_DEBUG ("Setting Autopurge " << m_killDead << "for: " << this);
	}
	if (m_status == Dead && on)
		emit finished( IConnection::Pointer( this ) );
}
/*----------------------------------------------------------------------------*/
const IConnection::PacketCount Connection::waitingPackets() const
{
	return PacketCount( 0, 0 );
}
/*----------------------------------------------------------------------------*/
bool Connection::addPacket( const Packet& packet )
{
	{
		const NetworkInfo& packetInfo = packet.networkInfo();

		QWriteLocker lock( &m_guard );
		m_status = Alive;
		
		if MY_WAY (packetInfo)
		{
			m_data.enqueue( DirectedPacket( Forward, packet.data() ) );

			m_speedUp += packet.data().size();
			m_dataUp += packet.data().size();
			PRINT_DEBUG ("ADDED " << packet.data().size() << "BYTES");
			++m_countForward;
		} else

		if BACK_WAY (packetInfo)
		{
			m_data.enqueue( DirectedPacket( Back, packet.data() ) );

			m_speedDown += packet.data().size();
			m_dataDown += packet.data().size();
			PRINT_DEBUG ("ADDED " << packet.data().size() << "BYTES");
			++m_countBack;
		} else
		{
			Q_ASSERT (!"Wrong way packet");
		}

		while (m_data.count() > MAX_PACKETS_IN_QUEUE )
		{
			m_data.dequeue();
		}

	}
	emit packetArrived( IConnection::Pointer( this ) );
	return true;
}
