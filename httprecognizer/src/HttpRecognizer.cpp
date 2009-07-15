/*!
 * @file
 * @brief Short description.
 *
 * Long description. I would paste some Loren Ipsum rubbish here, but I'm afraid
 * It would stay that way. Not that this comment is by any means ingenious but
 * at least people can understand it.
 */
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
	try {
		const NetworkInfo& info = connection->networkInfo();
		if (info.sourcePort == HTTP_PORT || info.destinationPort == HTTP_PORT)
			return true;
	} catch (std::runtime_error error) {
		PRINT_DEBUG( "ERROR:" << error.what() );
	}
	return false;
}
/*---------------------------------------------------------------------------*/
bool HttpRecognizer::parse( IConnection* connection )
{
	Q_ASSERT( connection );
	try {
		mConnections[connection->networkInfo()] << connection->nextPacket();
		return true;
	} catch (std::runtime_error error) {
		PRINT_DEBUG( "ERROR:" << error.what() );
		return false;
	}
}
/*---------------------------------------------------------------------------*/
QVariant HttpRecognizer::comment( IConnection* connection )
{
	Q_ASSERT (connection);
	if (!mConnections.contains( connection->networkInfo() ))
		return "No comment yet";

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
