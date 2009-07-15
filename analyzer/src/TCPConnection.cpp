/*!
 * @file
 * @brief Short description.
 *
 * Long description. I would paste some Loren Ipsum rubbish here, but I'm afraid
 * It would stay that way. Not that this comment is by any means ingenious but
 * at least people can understand it.
 */
#include "TCPConnection.h"

bool TCPConnection::addPacket( const Packet& packet )
{
	const bool success = Connection::addPacket( packet );
	if ( success && packet.isLast() )
		close();
	return success;
}
