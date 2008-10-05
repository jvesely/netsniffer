#include "PacketSorter.h"
void PacketSorter::run(){
	//loop.exec();
	while (cont)
		packet();
}

void PacketSorter::packet(){
	//while (cont){
//		if (packets->empty()) continue;
		Packet * packet = NULL;
		try{
			QByteArray data = packets->dequeue();
			packet = new Packet(data);
		}catch (std::runtime_error err){
			//qWarning() << err.what();
			return;
		}
		if (!packet) return; //continue;

		qDebug() << "Running";
	  QPointer<Connection>  &con = (*connections)[*packet];
  	if ( !con ) { 
			//null (deleted or just constructed)
			con = new Connection(*packet);
			//connect (this, SIGNAL(sendAutoPurge(bool)), con, SLOT(setAutoPurge(bool)));
			//model_.insertConnection(con);
			emit connection(con);
			qDebug() << "Added connection: " << connections->count();
		} else
			(*con) << *packet;
	
		delete packet;
	//}
	//qDebug() << "done sorting";
}
/*----------------------------------------------------------------------------*/
void PacketSorter::stop(){
//loop.exit();
	cont = false;
}
