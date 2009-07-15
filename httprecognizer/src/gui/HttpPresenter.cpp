#include "HttpPresenter.h"

#define DEBUG_TEXT "[ HttpPresenter ]:"
#include "debug.h"

HttpPresenter::HttpPresenter( const ConnectionTable& connections )
: mModel( connections.values() )
{
	setupUi( this );
	connectionsView->setModel( &mModel );
}
