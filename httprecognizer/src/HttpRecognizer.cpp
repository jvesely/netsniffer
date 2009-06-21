#include "HttpRecognizer.h"
#include "IConnection.h"

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
	return false;
}
/*---------------------------------------------------------------------------*/
QVariant HttpRecognizer::comment( IConnection* connection )
{
	return "Recognized by HttpRecognizer";
}
/*---------------------------------------------------------------------------*/
bool HttpRecognizer::showDetails( IConnection* connection )
{
	return false;
}
