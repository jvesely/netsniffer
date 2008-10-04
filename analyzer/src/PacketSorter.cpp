#include "PacketSorter.h"

void PacketSorter::run(){
	while (cont){
		if (packets.empty()) continue;
		QByteArray data = packets.dequeue();
		Packet * packet = NULL;
		try{
			packet = new Packet();
		}catch (std::runtime_error err){
			qWarning() << err.what();
		}
		if (!packet) continue;

		qDebug() << "Running";
		delete packet;
	}
	qDebug() << "done sorting";
}
/*----------------------------------------------------------------------------*/
void PacketSorter::stop(){
	cont = false;
}
