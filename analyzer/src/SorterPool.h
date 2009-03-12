#include "Packet.h"
#include "SafeQueue.h"
#include "SafeHash.h"
#include "PacketSorter.h"
#include "Connection.h"

class SorterPool:public QThreadPool{

	Q_OBJECT;

	SafeQueue<QByteArray>  packets;
	SafeHash<Packet, QPointer<Connection> >  connections;
	QList<PacketSorter *> sorters;
	QMutex guard;

public:
//	SorterPool();
/*		SafeHash<Packet, QPointer<Connection> >  *conns,
		SafeQueue<QByteArray> * data
	):packets(data),connections(conns){};*/
	~SorterPool() throw();
	void addPacket(QByteArray data) throw();
	void addThreads(int n = 1) throw();
	void removeThreads(int n = 1) throw();

signals:
	void connection(Connection * conn);

private slots:
	void addConnection(Connection * conn)throw(){
		emit connection(conn);
	}

};
