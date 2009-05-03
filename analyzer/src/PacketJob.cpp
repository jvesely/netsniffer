#include "PacketJob.h"
#include "Analyzer.h"
#include "TCPConnection.h"
#include "UDPConnection.h"

#define DEBUG_TEXT "[ PacketJob ]: "
#include "debug.h"

void PacketJob::run()
{
	try {
		PRINT_DEBUG << "Job started..";
		const Packet packet( m_data );
		QSharedPointer<Connection> new_connection =
			m_connections.value( packet.networkInfo() );
		if ( !new_connection )
		{
			switch (packet.networkInfo().protocol)
			{
				case TCP:
					new_connection = QSharedPointer<Connection>( new TCPConnection( packet ) );
					break;
				case UDP:
					new_connection = QSharedPointer<Connection>( new UDPConnection( packet ) );
					break;
				default:
					Q_ASSERT(!"Only TCP/UDP Connections are allowed.");
			}
			m_connections[ packet.networkInfo() ] = new_connection;
			ANALYZER->addConnection( new_connection.data() ); 
			PRINT_DEBUG << "Added Connection. TOTAL: " << m_connections.count();
		} else {
			*new_connection << packet;
			PRINT_DEBUG << "Merged to existing connection.";
		}
	}
	catch (std::runtime_error err) {
		PRINT_DEBUG << "Error creating packet instance: " << err.what();
	}
}
