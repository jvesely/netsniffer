#include "Packet.h"
#include "SafeQueue.h"
#include "SafeHash.h"
#include "PacketSorter.h"
#include "Connection.h"

typedef	SafeQueue<QByteArray>  DataQueue;
typedef SafeHash<Packet, QPointer<Connection> > ConnectionTable;
typedef QList<PacketSorter *> SorterList;

class SorterPool:public QThreadPool
{

	Q_OBJECT;

public:
//	SorterPool();
/*		SafeHash<Packet, QPointer<Connection> >  *conns,
		SafeQueue<QByteArray> * data
	):packets(data),connections(conns){};*/
	~SorterPool() throw();
	void addPacket(QByteArray data) throw();
	void addThreads(uint n = 1) throw();
	void removeThreads(uint n = 1) throw();

signals:
	void connection(Connection * conn);

private slots:
	void addConnection(Connection * conn)throw()
		{ emit connection(conn); }

private:
	DataQueue  m_waitingPackets;
	ConnectionTable  m_existingConnections;
	SorterList m_availableSorters;
//	QMutex guard;

};
