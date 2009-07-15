#pragma once

#include "IConnection.h"
#include "Http.h"

class HttpConnectionData: public QSharedData
{
public:
	HttpConnectionData():mStatus( ExpectingRequest ), mResponseData( NULL ){};
	
	void addPacket( IConnection::Direction direction, QByteArray packet );

	inline const QHttpRequestHeader lastRequestHeader() const
		{ return mLastRequest.first; }

	inline const QHttpResponseHeader lastResponseHeader() const
		{ return mLastResponseHeader; }
	
	inline const Http::Session& session() const
		{ return mSession; }

private:
	enum Status
		{ ExpectingRequest, RecievedRequestHeaders, RecievedResponseHeaders };
	Status mStatus;
	Http::Request mLastRequest;
	QHttpResponseHeader mLastResponseHeader;
	Http::Session mSession;
	IConnection::Direction mRequestDirection;
	QSharedPointer<QByteArray> mResponseData;
};

/*---------------------------------------------------------------------------*/
inline uint qHash( const QHttpRequestHeader& header )
{
	return qHash( header.toString() );
}
/*---------------------------------------------------------------------------*/
inline bool operator == ( const QHttpRequestHeader& a, const QHttpRequestHeader& b )
{
	return a.toString() == b.toString();
}
