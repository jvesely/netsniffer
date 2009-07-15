#pragma once

#include "IConnection.h"

class HttpConnectionData: public QSharedData
{
public:
	typedef QPair<QHttpResponseHeader, QByteArray> Response;
	typedef QPair<QHttpRequestHeader, QByteArray> Request;
	typedef QHash<Request, Response> HttpDialogue;

	HttpConnectionData():mStatus( ExpectingRequest ), mResponseData( NULL ){};
	
	void addPacket( IConnection::Direction direction, QByteArray packet );

	inline const QHttpRequestHeader lastRequestHeader() const
		{ return mLastRequest.first; }

	inline const QHttpResponseHeader lastResponseHeader() const
		{ return mLastResponseHeader; }
	
	inline const HttpDialogue& dialogue() const
		{ return mDialogue; }

private:
	enum Status
		{ ExpectingRequest, RecievedRequestHeaders, RecievedResponseHeaders };
	Status mStatus;
	Request mLastRequest;
	QByteArray* mResponseData;
	QHttpResponseHeader mLastResponseHeader;
	HttpDialogue mDialogue;
	IConnection::Direction mRequestDirection;
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
