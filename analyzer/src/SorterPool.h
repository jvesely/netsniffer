#include "Packet.h"
#include "SafeQueue.h"
#include "PacketSorter.h"

class SorterPool:private QThreadPool{
	SafeQueue<QByteArray> packets;
	QList<PacketSorter *> sorters;

public:
	~SorterPool();
	void addPacket(QByteArray data) throw();
	void addThreads(int n = 1) throw();
	void removeThreads(int n = 1) throw();
};
