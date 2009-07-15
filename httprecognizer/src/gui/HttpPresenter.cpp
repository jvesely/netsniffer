#include "HttpPresenter.h"
#include "HttpGlobalCache.h"

#define DEBUG_TEXT "[ HttpPresenter ]:"
#include "debug.h"

HttpPresenter::HttpPresenter( const HttpRecognizer::ConnectionTable& connections )
: mModel( connections.values() )
{
	setupUi( this );
	connectionsView->setModel( &mModel );

	connect( connectionsView, SIGNAL(clicked( const QModelIndex& )),
		this, SLOT( selectResource( const QModelIndex& )) );
}
/*----------------------------------------------------------------------------*/
void HttpPresenter::selectResource( const QModelIndex& index )
{
	PRINT_DEBUG( "selected resource" << index );
	if (!index.isValid() || !index.internalId())
		return;

	Q_ASSERT (connectionsView);
	Q_ASSERT (webView);

	const HttpConnection connection = mModel.getConnection( index.parent() );
	Q_ASSERT (index.row() < connection.session().count() );
	const Http::Request request = connection.session().at( index.row() ).first;
	const Http::Response* response = HTTP_CACHE[ request ];

	if (response)
	{
		PRINT_DEBUG( "PRINTING:" << *response->second );
		webView->stop();
		webView->setContent( *response->second, response->first.value( "content-type" ) );
	} else {
		PRINT_DEBUG( "NO RESPONSE CACHED" );
	}
}
