#include "IConnection.h"

class HttpConnection
{
public:
	typedef QPair<QHttpResponseHeader, QByteArray> Response;
	typedef QPair<QHttpRequestHeader, QByteArray> Request;
	typedef QHash<Request, Response> HttpDialogue;

	HttpConnection():mStatus( ExpectingRequest ){};
	inline HttpConnection& operator << ( IConnection::DirectedPacket packet )
		{ addPacket( packet.first, packet.second ); return *this; }
	void addPacket( IConnection::Direction direction, QByteArray packet );

	inline const QHttpRequestHeader lastRequestHeader()
		{ return mLastRequest.first; }

	inline const QHttpResponseHeader lastResponseHeader()
		{ return mLastResponseHeader; }
	
	inline const HttpDialogue& dialogue()
		{ return mDialogue; }


private:
	enum Status
		{ ExpectingRequest, RecievedRequestHeaders, RecievedResponseHeaders };
	Status mStatus;
	Request mLastRequest;
	QHttpResponseHeader mLastResponseHeader;
	HttpDialogue mDialogue;
	IConnection::Direction mRequestDirection;
};

inline uint qHash( const QHttpRequestHeader& header )
{
	return qHash( header.toString() );
}

inline bool operator == ( const QHttpRequestHeader& a, const QHttpRequestHeader& b )
{
	return a.toString() == b.toString();
}
