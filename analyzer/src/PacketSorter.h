#include "struct/SafeQueue.h"
#include "struct/SafeHash.h"
#include "Packet.h"
#include "Connection.h"

typedef SafeQueue<QByteArray>  DataQueue;
typedef SafeHash<Packet, QPointer<Connection> > ConnectionTable;


class PacketSorter:public QThread
{
	
	Q_OBJECT;

public:
	PacketSorter( DataQueue* packetqueue, ConnectionTable* conns )
	:m_run( true ), m_packets( packetqueue ), m_connections( conns ) {};
	void run();
	void stop();

public slots:
	void processPacket();

signals:
	void connection( Connection * );

private:
	volatile bool m_run;
	DataQueue* m_packets;
	ConnectionTable* m_connections;
};
