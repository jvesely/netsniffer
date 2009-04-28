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
