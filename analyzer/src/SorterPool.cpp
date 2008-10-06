#include "SorterPool.h"

#define THREAD_LIMIT 10

/*----------------------------------------------------------------------------*/
void SorterPool::addPacket(QByteArray data) throw() {
/*	Packet * packet = NULL;
  try{
    packet = new Packet(data);
  }catch (std::runtime_error err){
    qWarning() << err.what();
  }
  if (!packet) return;
*/
	packets.enqueue(data);
	//emit packet();
	//qDebug() << "Packet count: " << packets->count();
}
/*----------------------------------------------------------------------------*/
void SorterPool::addThreads(int n) throw(){
	if (n <= 0 || n > THREAD_LIMIT) return;
	while(n){
		qDebug() << "Adding sorting thread";
		PacketSorter * sorter =  new PacketSorter(&packets, &connections);
		if (!sorter) return; //terribly wrong
		connect(sorter, SIGNAL(connection(Connection*)), this, SIGNAL(connection(Connection *)), Qt::DirectConnection);
		connect(this, SIGNAL(packet()), sorter, SLOT(packet()), Qt::QueuedConnection);
		sorters.append(sorter);
		start(sorter);
		--n;
	}
}
/*----------------------------------------------------------------------------*/
void SorterPool::removeThreads(int n) throw(){
	if (sorters.isEmpty() || n > sorters.count()) return;
	while (n){
		qDebug() << "Removing sorter from pool of " << sorters.count();
		PacketSorter * sorter = sorters.takeFirst();
		sorter->stop(); // ThreadPoolAutomatically deletes it whne it finishes
		--n;
	}
}
/*----------------------------------------------------------------------------*/
SorterPool::~SorterPool(){
	qDebug() << "Killing sorting workers...";
//	int count = sorters.count();
	while(!sorters.isEmpty())
		sorters.takeFirst()->stop();
	// ok no more sorters in my list
	packets.release(); // just to make sure threads will wake up
	waitForDone();
}
