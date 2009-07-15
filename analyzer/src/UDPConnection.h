/*!
 * @file
 * @brief Short description.
 *
 * Long description. I would paste some Loren Ipsum rubbish here, but I'm afraid
 * It would stay that way. Not that this comment is by any means ingenious but
 * at least people can understand it.
 */
#include "Connection.h"

class UDPConnection: public Connection
{
public:
	UDPConnection( const Packet& packet ):
		Connection( packet ), mDeathTimeout( 0 ) {};
	
	virtual bool addPacket( const Packet& packet );
	virtual void update();

private:
	uint mDeathTimeout;
	Q_DISABLE_COPY (UDPConnection);
};
