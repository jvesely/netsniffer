#include "PacketSorter.h"

#define DEBUG_TEXT "[ PacketSorter ]:"
#include "debug.h"

void PacketSorter::run()
{
	while (cont)
		processPacket();
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

	  QPointer<Connection>  &con = (*m_connections)[*packet];
  	if ( !con )
		{ 
			//null (deleted or just constructed)
			con = new Connection(*packet);
			emit connection(con);
			PRINT_DEBUG << "Added Connection. TOTAL: " << m_connections->count() ;		
		} else
			(*con) << *packet;
	
		delete packet;
}
/*----------------------------------------------------------------------------*/
void PacketSorter::stop()
{
	cont = false;
}
