#include "HttpRecognizer.h"
#include "HttpConnection.h"
#include "IConnection.h"

#include "gui/HttpPresenter.h"

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
	mConnections[connection->networkInfo()] << connection->nextPacket();
	return true;
}
/*---------------------------------------------------------------------------*/
QVariant HttpRecognizer::comment( IConnection* connection )
{
	Q_ASSERT (connection);
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

	HttpPresenter presenter( mConnections );
	presenter.exec();
	
	return true;
}
