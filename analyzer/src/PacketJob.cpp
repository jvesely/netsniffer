#include "PacketJob.h"
#include "Analyzer.h"
#include "TCPConnection.h"
#include "UDPConnection.h"

#define DEBUG_TEXT "[ PacketJob ]: "
#include "debug.h"

void PacketJob::run()
{
	try {
		const Packet packet( m_data );
		QPointer<Connection> &new_connection = m_connections[packet];
		if ( !new_connection )
		{
			switch (packet.networkInfo().protocol)
			{
				case TCP:
					new_connection = new TCPConnection( packet );
					break;
				case UDP:
					new_connection = new UDPConnection( packet );
					break;
				default:
					Q_ASSERT(!"Only TCP/UDP Connections are allowed.");
			}
			ANALYZER->addConnection( new_connection ); 
			PRINT_DEBUG << "Added Connection. TOTAL: " << m_connections.count();
		} else
			*new_connection << packet;
	}
	catch (std::runtime_error err) {
		PRINT_DEBUG << "Error creating packet instance: " << err.what();
	}
}
