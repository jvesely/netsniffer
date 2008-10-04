#include "SafeQueue.h"
#include "Packet.h"

class PacketSorter: public QRunnable, public QObject{

	volatile bool cont;
	SafeQueue<QByteArray> &packets;

public:
	PacketSorter(SafeQueue<QByteArray> &packetqueue):cont(true), packets(packetqueue){};
	void run();
	void stop();

};
