#pragma once

#include "IConnection.h"
#include "HttpConnectionData.h"

class HttpConnection: public QExplicitlySharedDataPointer<HttpConnectionData>
{
public:
	typedef QPair<QHttpResponseHeader, QByteArray> Response;
	typedef QPair<QHttpRequestHeader, QByteArray> Request;
	typedef QHash<Request, Response> HttpDialogue;

	HttpConnection()
	:	QExplicitlySharedDataPointer<HttpConnectionData>( new HttpConnectionData )
	{ Q_ASSERT (data()); };

	inline HttpConnection& operator << ( IConnection::DirectedPacket packet )
		{ addPacket( packet.first, packet.second ); return *this; }

	inline void addPacket( IConnection::Direction direction, QByteArray packet )
		{ data()->addPacket( direction, packet ); }

	inline const QHttpRequestHeader lastRequestHeader() const
		{ return data()->lastRequestHeader(); }

	inline const QHttpResponseHeader lastResponseHeader() const
		{ return data()->lastResponseHeader(); }
	
	inline const HttpDialogue& dialogue() const
		{ return data()->dialogue(); }
};
