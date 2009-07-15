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
