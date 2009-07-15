/*!
 * @file
 * @brief Short description.
 *
 * Long description. I would paste some Loren Ipsum rubbish here, but I'm afraid
 * It would stay that way. Not that this comment is by any means ingenious but
 * at least people can understand it.
 */
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
