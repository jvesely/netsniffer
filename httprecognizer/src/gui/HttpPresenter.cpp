#include "HttpPresenter.h"

#define DEBUG_TEXT "[ HttpPresenter ]:"
#include "debug.h"

HttpPresenter::HttpPresenter( const ConnectionTable& connections )
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
	Q_ASSERT (index.row() < connection.dialogue().count() );
	const HttpConnection::Response response = connection.dialogue().values()[index.row()];

	PRINT_DEBUG( "PRINTING:" << response.second );
	webView->stop();
	webView->setContent( response.second, response.first.value( "content-type" ) );
}
