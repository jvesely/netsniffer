#include "HttpConnection.h"

#define DEBUG_TEXT "[ HttpConnection ]:"
#include "debug.h"

static const QByteArray HEADER_SEPARATOR = "\r\n\r\n";

void HttpConnection::addPacket( IConnection::Direction direction, QByteArray packet )
{
	//PRINT_DEBUG( "get data" << packet ); 
	if (packet.isEmpty())
		return;
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
				if (header.statusCode() / 100 == 2)
				{
					mDialogue[mLastRequest].first = header;
				}
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
				//response data
				break;
			}
	}
}
