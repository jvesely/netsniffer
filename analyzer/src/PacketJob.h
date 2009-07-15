#pragma once
#include "Analyzer.h"
#include "Connection.h"

class PacketJob: public QRunnable
{
public:
	inline PacketJob( const QByteArray& data, ConnectionTable& connections )
		: mData( data ), mConnections( connections ) {};
	virtual void run();

private:
	const QByteArray mData;
	ConnectionTable& mConnections;
};
