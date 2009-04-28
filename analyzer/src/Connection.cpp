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

#define FORWARD true
#define BACKWARD false

Connection::Connection( const Packet& packet ):
	m_info(packet.networkInfo()),	m_countForward( 1 ), m_countBack( 0 ),
	m_timeout( TIMEOUT_INTERVAL ), m_deathTime( 0 ),
	m_speedDown( 0 ), m_dataDown( 0 ), 
	m_status( Alive )//,	m_deathTimer( this )
{
	
	m_data.append( DirectedData( FORWARD, packet.data() ) );
	m_lastPacketForward = packet.data();
	m_speedUp = m_dataUp = m_lastPacketForward.size();
	
	if (m_info.protocol == UDP){
		//connect( &m_deathTimer, SIGNAL(timeout()), this, SLOT(close()) );
		//connect( this, SIGNAL(restartTimer()), &m_deathTimer, SLOT(start()) );
		//m_deathTimer.start();
	}
}
/*----------------------------------------------------------------------------*/
Connection::~Connection()
{
	emit destroyed( this );
	disconnect();
	QWriteLocker locker(&m_guard); // wait if something is by any chance inserting packet
}
/*----------------------------------------------------------------------------*/
void Connection::update()
{
	{
		QWriteLocker lock( &m_guard );
	
		m_speedUp = m_dataUp;
		m_speedDown = m_dataDown;
		m_dataUp = m_dataDown = 0;
	
		if (++m_deathTime >= m_timeout) {
			close();
			m_deathTime = 0;
			emit statusChanged( this ); // to force redraw
		}
		
	}
}
/*----------------------------------------------------------------------------*/
void Connection::close()
{
	m_status = Closed;
	QTimer::singleShot( TIMEOUT_INTERVAL * 1000, this, SLOT(die()) );
	PRINT_DEBUG << "Closed connection.." << this;
}
/*----------------------------------------------------------------------------*/
void Connection::die()
{
	if (m_status != Closed)
		return;

	PRINT_DEBUG << "Connection dying";
	m_status = Dead;

	if (m_killDead)
		deleteLater();
	else
		emit statusChanged( this );
}
/*----------------------------------------------------------------------------*/
void Connection::setAutoPurge( bool on )
{
	m_killDead = on;
	PRINT_DEBUG << "Autopurge " << m_killDead;
	if (m_status == Dead && on)
		deleteLater();
}
/*----------------------------------------------------------------------------*/
Connection& Connection::addPacket( const Packet& packet )
{
	{
		NetworkInfo packetInfo = packet.networkInfo();

		QWriteLocker lock( &m_guard );
		m_status = Alive;
		
		if MY_WAY (packetInfo)
		{
			m_lastPacketForward = packet.data();
			m_data.append( DirectedData( true, m_lastPacketForward ) );

			m_dataUp += m_lastPacketForward.count();
			++m_countForward;
			
			goto end;
		}
		
		if BACK_WAY (packetInfo)
		{
			m_lastPacketBack = packet.data();
			m_data.append( DirectedData( false, m_lastPacketBack ) );

			m_dataDown += m_lastPacketBack.count();
			++m_countBack;

			goto end;
		}

		Q_ASSERT (false);

end:
		if (m_info.protocol == TCP && packet.isLast())
			close();
//		if (m_info.protocol == UDP)
			//m_deathTimer.start();
	m_deathTime = 0;
	}
	emit packetArrived( this );
	return *this;
}
