/*!
 * @file
 * @brief Short description.
 *
 * Long description. I would paste some Loren Ipsum rubbish here, but I'm afraid
 * It would stay that way. Not that this comment is by any means ingenious but
 * at least people can understand it.
 */
#pragma once

#include "IConnection.h"
#include "HttpConnectionData.h"

class HttpConnection: public QExplicitlySharedDataPointer<HttpConnectionData>
{
public:

	HttpConnection()
	:	QExplicitlySharedDataPointer<HttpConnectionData>( new HttpConnectionData )
	{ Q_ASSERT (data()); };

	inline HttpConnection& operator << ( IConnection::DirectedPacket packet )
		{ addPacket( packet.first, packet.second ); return *this; }

	inline void addPacket( IConnection::Direction direction, const QByteArray packet )
		{ data()->addPacket( direction, packet ); }

	inline const QHttpRequestHeader lastRequestHeader() const
		{ return data()->lastRequestHeader(); }

	inline const QHttpResponseHeader lastResponseHeader() const
		{ return data()->lastResponseHeader(); }
	
	inline const Http::Session& session() const
		{ return data()->session(); }
};
