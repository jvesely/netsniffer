#include "PacketJob.h"
#include "Analyzer.h"
#include "TCPConnection.h"
#include "UDPConnection.h"

#define DEBUG_TEXT "[ PacketJob ]: "
#include "debug.h"

void PacketJob::run()
{
	try {
		PRINT_DEBUG ("Job started..");
		const Packet packet = Packet::parse( m_data );
		Connection::Pointer connection = m_connections.value( packet.networkInfo() );
		if ( !connection )
		{
			switch (packet.networkInfo().protocol)
			{
				case TCP:
					connection = Connection::Pointer( new TCPConnection( packet ) );
					break;
				case UDP:
					connection = Connection::Pointer( new UDPConnection( packet ) );
					break;
				default:
					Q_ASSERT(!"Only TCP/UDP Connections are allowed.");
			}
			ANALYZER.addConnection( connection ); 
			PRINT_DEBUG ("Added Connection. TOTAL: " << m_connections.count());
		} else {
			*connection << packet;
			PRINT_DEBUG ("Merged to existing connection.");
		}
	}
	catch (std::runtime_error err) {
		PRINT_DEBUG ("Error creating packet instance: " << err.what());
	}
}
