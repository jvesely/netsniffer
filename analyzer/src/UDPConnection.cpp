#include "UDPConnection.h"

bool UDPConnection::addPacket( const Packet& packet )
{
	m_deathTimeout = 0;
	return Connection::addPacket( packet );
}
/*----------------------------------------------------------------------------*/
void UDPConnection::update()
{
	Connection::update();
	++m_deathTimeout;
	if (mStatus == Alive && m_deathTimeout >= mTimeout)
		close();
}
