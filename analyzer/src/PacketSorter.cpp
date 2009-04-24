#include "PacketSorter.h"

#define DEBUG_TEXT "[ PacketSorter ]:"
#include "debug.h"

void PacketSorter::run()
{
	while (m_run)
		processPacket();
	deleteLater();
}

void PacketSorter::processPacket()
{
	//while (cont){
//		if (packets->empty()) continue;
		Packet* packet = NULL;

		try {
			QByteArray data = m_packets->dequeue();
			packet = new Packet(data);
		} catch (std::runtime_error err) {
			PRINT_DEBUG << "Error creating packet instance: " << err.what();
			return;
		}
//		if (!packet) return; //continue;
		Q_ASSERT (packet);

	  QPointer<Connection>  &new_connection = (*m_connections)[*packet];
  	if ( !new_connection )
		{ 
			//null (deleted or just constructed)
			new_connection = new Connection(*packet);
			emit connection( new_connection );
			PRINT_DEBUG << "Added Connection. TOTAL: " << m_connections->count() ;		
		} else
			(*new_connection) << *packet;
	
		delete packet;
}
/*----------------------------------------------------------------------------*/
void PacketSorter::stop()
{
	m_run = false;
}
