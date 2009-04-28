#pragma once
#include "Packet.h"
#include "Connection.h"
#include "struct/SafeHash.h"

typedef SafeHash<Packet, QPointer<Connection> > ConnectionTable;

class PacketJob: public QRunnable
{
public:
	inline PacketJob( const QByteArray& data, ConnectionTable& connections )
		: m_data( data ), m_connections( connections ) {};
	virtual void run();

private:
	const QByteArray m_data;
	ConnectionTable& m_connections;

};
