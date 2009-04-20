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


Connection::Connection(const Packet& packet):
	m_info(packet.networkInfo()),	m_countForward( 1 ), m_countBack( 0 ),
	m_timeout( TIMEOUT_INTERVAL ), m_deathTime( 0 ),
	m_speedDown( 0 ), m_dataDown( 0 ), 
	m_status( Cs_Alive )//,	m_deathTimer( this )
{
	qRegisterMetaType<ConnectionField>("ConnectionField");
	
//	m_sourceName = m_info.sourceIP.toString();
//	m_destinationName = m_info.destinationIP.toString();

	m_data.append( DirectedData( true, packet.data() ) );

	m_lastPacketForward = packet.data();

	m_speedUp = m_dataUp = m_lastPacketForward.size();
	
//	m_deathTimer.setSingleShot( true );
//	m_deathTimer.setInterval( m_timeout );
	
	if (m_info.protocol == UDP){
		//connect( &m_deathTimer, SIGNAL(timeout()), this, SLOT(close()) );
		//connect( this, SIGNAL(restartTimer()), &m_deathTimer, SLOT(start()) );
		//m_deathTimer.start();
	}
}
/*----------------------------------------------------------------------------*/
Connection::~Connection()
{
	emit destroyed(this);
	disconnect();
	QWriteLocker locker(&m_guard); // wait if something is by any chance inserting packet
}
/*----------------------------------------------------------------------------*/
void Connection::update( const QCache<QHostAddress, QString> * dns )
{
	int changed_fields = Cf_Speed;
	{
		QWriteLocker lock( &m_guard );
	
/*		if (dns) {
			QString * names = NULL;
			m_sourceName = 
				(names = dns->object( m_info.sourceIP )) ? (*names) : m_info.sourceIP.toString();
			m_destinationName = 
				(names = dns->object( m_info.destinationIP )) ? (*names) : m_info.destinationIP.toString();
		
			if (names)
				address = Cf_Address;
		}
*/
		m_speedUp = m_dataUp;
		m_speedDown = m_dataDown;
		m_dataUp = m_dataDown = 0;
	
		if (++m_deathTime >= m_timeout) {
			close();
			m_deathTime = 0;
			changed_fields |= Cf_Status;
		}
		
	}
	emit changed( this, changed_fields ); // to force redraw
}
/*----------------------------------------------------------------------------*/
void Connection::close()
{
	m_status = Cs_Closed;
//	emit changed( this, Cf_Status );
	QTimer::singleShot( TIMEOUT_INTERVAL * 1000, this, SLOT(die()) );
	PRINT_DEBUG << "Closed connection.." << this;
}
/*----------------------------------------------------------------------------*/
void Connection::die()
{
	if (m_status != Cs_Closed)
		return;

	PRINT_DEBUG << "Connection dying";
	m_status = Cs_Dead;

	if (m_killDead)
		deleteLater();
	else
		emit changed( this, Cf_Status );
}
/*----------------------------------------------------------------------------*/
/*void Connection::setQuick( QPair<QString, QString> desc )
{
	Q_UNUSED( desc );
	//shortDescFw = desc.first;
	//shortDescBc =  desc.second;
	//emit changed(this, Cf_Comment);
}
*/
/*----------------------------------------------------------------------------*/
/*void Connection::purge(){

	if (status != Cs_Closed)
		return;
//	qDebug() << this <<": I'm about to die " ;
//	emit timedOut(this);
	deleteLater();
//	reset();
//	emit timedOut(this);
}*/
/*----------------------------------------------------------------------------*/
void Connection::setAutoPurge( bool on )
{
	m_killDead = on;
	PRINT_DEBUG << "Autopurge " << m_killDead;
	if (m_status == Cs_Dead && on)
		deleteLater();
}
/*----------------------------------------------------------------------------*/
Connection& Connection::operator << ( const Packet& packet )
{
	{
		NetworkInfo packetInfo = packet.networkInfo();

		QWriteLocker lock( &m_guard );
		m_status = Cs_Alive;
		
		if MY_WAY(packetInfo)
		{
			m_lastPacketForward = packet.data();
			m_data.append( DirectedData( true, m_lastPacketForward ) );

			m_dataUp += m_lastPacketForward.count();
			++m_countForward;
			
			goto end;
		}
		
		if BACK_WAY(packetInfo)
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
	}
	m_deathTime = 0;
	emit changed( this, Cf_PacketCount );
	return *this;
}
