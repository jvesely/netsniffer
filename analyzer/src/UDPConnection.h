#include "Connection.h"

class UDPConnection: public Connection
{
public:
	UDPConnection( const Packet& packet ):
		Connection( packet ) {};
	
	virtual bool addPacket( const Packet& packet );
	virtual void update();

private:
	uint m_deathTimeout;
	Q_DISABLE_COPY (UDPConnection);
};
