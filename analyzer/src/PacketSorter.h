#include "SafeQueue.h"
#include "SafeHash.h"
#include "Packet.h"
#include "Connection.h"

typedef SafeQueue<QByteArray>  DataQueue;
typedef SafeHash<Packet, QPointer<Connection> > ConnectionTable;


class PacketSorter:public QObject, public QRunnable
{
	
	Q_OBJECT;

public:
	PacketSorter( DataQueue* packetqueue, ConnectionTable* conns)
	:cont( true ), m_packets( packetqueue ), m_connections( conns ){};
	void run();
	void stop();

public slots:
	void processPacket();

signals:
	void connection( Connection * );

private:
	volatile bool cont;
	DataQueue* m_packets;
	ConnectionTable* m_connections;
};
