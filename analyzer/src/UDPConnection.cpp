/*!
 * @file
 * @brief Short description.
 *
 * Long description. I would paste some Loren Ipsum rubbish here, but I'm afraid
 * It would stay that way. Not that this comment is by any means ingenious but
 * at least people can understand it.
 */
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
