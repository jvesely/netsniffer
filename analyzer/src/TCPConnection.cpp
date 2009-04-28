#include "TCPConnection.h"

bool TCPConnection::addPacket( const Packet& packet )
{
	const bool success = Connection::addPacket( packet );
	if ( success && packet.isLast() )
		close();
	return success;
}
