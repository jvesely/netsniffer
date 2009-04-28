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
	if (m_status == Alive && m_deathTimeout >= m_timeout)
		close();
}
