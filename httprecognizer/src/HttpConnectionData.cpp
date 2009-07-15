#include "HttpConnection.h"
#include "HttpGlobalCache.h"

#define DEBUG_TEXT "[ HttpConnection ]:"
#include "debug.h"

static const QByteArray HEADER_SEPARATOR = "\r\n\r\n";

void HttpConnectionData::addPacket( IConnection::Direction direction, QByteArray packet )
{
	//PRINT_DEBUG( "get data" << packet ); 
	if (packet.isEmpty())
		return;

	QMutexLocker lock( &mGuard );
	switch (mStatus)
	{
		case ExpectingRequest:
		{
			PRINT_DEBUG( "Analyzing request headers" );
			const int headers_end = packet.indexOf( HEADER_SEPARATOR );
			const QByteArray headers_data = headers_end == -1 ? packet : packet.left( headers_end );
			QHttpRequestHeader header( headers_data );
//			PRINT_DEBUG( "RAW:" << packet );
			if (header.isValid())
			{
				PRINT_DEBUG( "PARSED:" << header.toString() );
				mLastRequest.first = header;
				mLastRequest.second.clear();
				mRequestDirection = direction;
			} else
				return; //it might be anything

			mStatus = RecievedRequestHeaders;
			PRINT_DEBUG( packet.size() << headers_end );
			packet.remove( 0, headers_end + HEADER_SEPARATOR.size() );
			if (packet.isEmpty())
				return;
		}

		case RecievedRequestHeaders:
			if (direction == mRequestDirection)
			{
				//these must be request data (post or put..)
				mLastRequest.second += packet;
				break;
			} else {
				PRINT_DEBUG( "Analyzing response headers or request data" );
				const int headers_end = packet.indexOf( HEADER_SEPARATOR );
				const QByteArray headers_data = headers_end == -1 ? packet : packet.left( headers_end );
				QHttpResponseHeader header( (QString)headers_data );
				if (!header.isValid())
				{
					break;
				} 
				PRINT_DEBUG( header.toString() );
				mStatus = RecievedResponseHeaders;
				mLastResponseHeader = header;
				packet.remove( 0, headers_end + HEADER_SEPARATOR.size() );
				mSession.append( qMakePair( mLastRequest, header ) );

				Http::Response* to_cache = new Http::Response( header, 
					QSharedPointer<QByteArray>( new QByteArray() ) );

				mResponseData = to_cache->second;
				QUrl url( "http://" );
				url.setHost( mLastRequest.first.value( "host" ) );
				url.setPath( mLastRequest.first.path() );

				HTTP_CACHE.insert( url, to_cache );
			}
		
		case RecievedResponseHeaders:
			if (direction == mRequestDirection)
			{
				const int headers_end = packet.indexOf( HEADER_SEPARATOR );
				const QByteArray headers_data = headers_end == -1 ? packet : packet.left( headers_end );
				QHttpRequestHeader header( headers_data );
				if (!header.isValid()) break;
				
				PRINT_DEBUG( "PARSED:" << header.toString() );
				mLastRequest.first = header;
				mLastRequest.second.clear();

				mStatus = RecievedRequestHeaders;
				break;
			} else {
				PRINT_DEBUG( "RESPONSE DATA" );
				if (mResponseData)
				{
					mResponseData->append( packet );
				}
				break;
			}
	}
}
