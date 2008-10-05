#include "SafeQueue.h"
#include "SafeHash.h"
#include "Packet.h"
#include "Connection.h"

class PacketSorter:public QObject, public QRunnable{
	
	Q_OBJECT;

	volatile bool cont;
	SafeQueue<QByteArray> * packets;
	SafeHash<Packet, QPointer<Connection> > * connections;
	//QEventLoop loop;

public:
	PacketSorter(SafeQueue<QByteArray> * packetqueue, SafeHash<Packet, QPointer<Connection> > * conns):cont(true), packets(packetqueue), connections(conns){};
//	~PacketSorter(){loop.exit();};
	void run();
	void stop();
public slots:
	void packet();

signals:
	void connection(Connection * conn);

};
