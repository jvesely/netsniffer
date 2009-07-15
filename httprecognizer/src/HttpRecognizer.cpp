#include "HttpRecognizer.h"
#include "HttpConnection.h"
#include "IConnection.h"

#define DEBUG_TEXT "[ HttpRecognizer ]:"
#include "debug.h"

static const int HTTP_PORT = 80;

bool HttpRecognizer::guess( const IConnection* connection )
{
	Q_ASSERT (connection);
	const NetworkInfo& info = connection->networkInfo();
	if (info.sourcePort == HTTP_PORT || info.destinationPort == HTTP_PORT)
		return true;
	return false;
}
/*---------------------------------------------------------------------------*/
bool HttpRecognizer::parse( IConnection* connection )
{
	Q_ASSERT( connection );
//	const NetworkInfo& info = connection->networkInfo();
//HttpConnection* con;
/*	if (!(con = mConnections[info]))
	{
		con = mConnections[info] = new HttpConnection;
		PRINT_DEBUG( "created connection:" << con );
		PRINT_DEBUG( "total connections: " << mConnections.count());
	}
	
	Q_ASSERT( con );
	IConnection::DirectedPacket packet = connection->nextPacket();
	(*con) << packet;
*/
	mConnections[connection->networkInfo()] << connection->nextPacket();
	return true;
}
/*---------------------------------------------------------------------------*/
QVariant HttpRecognizer::comment( IConnection* connection )
{
	Q_ASSERT( connection );
	Q_ASSERT (mConnections.contains( connection->networkInfo() ));
	const HttpConnection con = mConnections.value( connection->networkInfo() );
	const QHttpRequestHeader request = con.lastRequestHeader();
	const QHttpResponseHeader response = con.lastResponseHeader();

	return request.method() + " " + request.value( "host" ) + request.path()
		+ (response.isValid() ? "\nHTTP " + QString::number( response.statusCode() ) + " " + response.reasonPhrase() : "" );
}
/*---------------------------------------------------------------------------*/
bool HttpRecognizer::showDetails( IConnection* connection )
{
	Q_ASSERT (connection);
	Q_ASSERT (mConnections.contains( connection->networkInfo() ));
	const HttpConnection con = mConnections.value( connection->networkInfo() );

	const HttpConnection::HttpDialogue dialogue = con.dialogue();

	PRINT_DEBUG (dialogue.count());
	for ( HttpConnection::HttpDialogue::const_iterator it = dialogue.begin();
		it != dialogue.end(); ++it)
	{
		PRINT_DEBUG( it.key().first.method() << it.key().first.path() << it.value().first.reasonPhrase() << it.value().second.count() );
	}
	
	return true;
}
