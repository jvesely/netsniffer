#include "SorterPool.h"

#define THREAD_LIMIT 10

/*----------------------------------------------------------------------------*/
void SorterPool::addPacket(QByteArray data) throw() {
	packets.enqueue(data);
}
/*----------------------------------------------------------------------------*/
void SorterPool::addThreads(int n) throw(){
	if (n <= 0 || n > THREAD_LIMIT) return;
	for(;n;--n){
		qDebug() << "Adding sorting thread";
		PacketSorter * sorter =  new PacketSorter(&packets, &connections);
		if (!sorter) return; //terribly wrong

		connect(sorter, SIGNAL(connection(Connection*)), this, SIGNAL(connection(Connection *)), Qt::DirectConnection);
		sorters.append(sorter);
		start(sorter);
	}
}
/*----------------------------------------------------------------------------*/
void SorterPool::removeThreads(int n) throw(){
	if (sorters.isEmpty() || n > sorters.count()) return;
	for (;n;--n){
		qDebug() << "Removing sorter from pool of " << sorters.count();
		PacketSorter * sorter = sorters.takeFirst();
		sorter->stop(); // ThreadPool automatically deletes finished threads
	}
}
/*----------------------------------------------------------------------------*/
SorterPool::~SorterPool() throw(){
	qDebug() << "Killing sorting workers...";
//	int count = sorters.count();
	while(!sorters.isEmpty())
		sorters.takeFirst()->stop();
	// ok no more sorters in my list
	packets.release(); // just to make sure threads will wake up
	waitForDone();
}
