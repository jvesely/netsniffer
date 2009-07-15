/*!
 * @file
 * @brief Short description.
 *
 * Long description. I would paste some Loren Ipsum rubbish here, but I'm afraid
 * It would stay that way. Not that this comment is by any means ingenious but
 * at least people can understand it.
 */
#include "Connection.h"


class TCPConnection: public Connection
{
public:
	TCPConnection( const Packet& packet ):
		Connection( packet ) {};
	
	bool addPacket( const Packet& packet );

private:
	Q_DISABLE_COPY (TCPConnection);
};
