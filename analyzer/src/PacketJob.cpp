#include "PacketJob.h"
#include "Analyzer.h"

void PacketJob::run()
{
	try {
		const Packet packet( m_data );
		QPointer<Connection> &new_connection = m_connections[packet];
		if ( !new_connection )
		{
			new_connection = new Connection( packet );
			ANALYZER->addConnection( new_connection ); 
			qDebug() << "Added Connection. TOTAL: " << m_connections.count();
		} else
			*new_connection << packet;
	}
	catch (std::runtime_error err) {
		qDebug() << "Error creating packet instance: " << err.what();
	}
}
