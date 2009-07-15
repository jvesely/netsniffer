/*!
 * @file
 * @brief Short description.
 *
 * Long description. I would paste some Loren Ipsum rubbish here, but I'm afraid
 * It would stay that way. Not that this comment is by any means ingenious but
 * at least people can understand it.
 */
#include "HttpPresenter.h"
#include "HttpGlobalCache.h"

#define DEBUG_TEXT "[ HttpPresenter ]:"
#include "debug.h"

HttpPresenter::HttpPresenter( const HttpRecognizer::ConnectionTable& connections )
: mModel( connections.values() )
{
	setupUi( this );
	connectionsView->setModel( &mModel );
	mAccessManager = new CacheAccessManager( &HTTP_CACHE );
	webView->page()->setNetworkAccessManager( mAccessManager );
	//webView->page()->networkAccessManager()->setCache( &HTTP_CACHE );


	connect( connectionsView, SIGNAL(clicked( const QModelIndex& )),
		this, SLOT( selectResource( const QModelIndex& )) );
}
/*----------------------------------------------------------------------------*/
void HttpPresenter::selectResource( const QModelIndex& index )
{
	if (!index.isValid() || !index.internalId())
		return;

	Q_ASSERT (connectionsView);
	Q_ASSERT (webView);

	const HttpConnection connection = mModel.getConnection( index.parent() );
	Q_ASSERT (index.row() < connection.session().count() );
	const Http::Request request = connection.session().at( index.row() ).first;

	QUrl url( "http://" );
  url.setHost( request.first.value( "host" ) );
  url.setPath( request.first.path() );
	PRINT_DEBUG( "REQUEST URL:" << url );
	webView->setUrl( url );
}
