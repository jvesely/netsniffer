#include "UDPConnection.h"

bool UDPConnection::addPacket( const Packet& packet )
{
	mDeathTimeout = 0;
	return Connection::addPacket( packet );
}
/*----------------------------------------------------------------------------*/
void UDPConnection::update()
{
	Connection::update();
	++mDeathTimeout;
	if (mStatus == Alive && mDeathTimeout >= mTimeout)
		close();
}
