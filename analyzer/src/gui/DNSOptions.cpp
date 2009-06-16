#include "DNSOptions.h"
#include "DNSCacheModel.h"

const QString DNSOptions::MY_NAME( "DNS Cache" );

bool DNSOptions::deploy( QWidget* container )
{
	Q_ASSERT (m_dns);
	setupUi( container );
	
	view->addAction( actionRemoveSelected );
	view->addAction( actionRemoveAll );
	view->setContextMenuPolicy( Qt::ActionsContextMenu );

	connect( actionRemoveSelected, SIGNAL(triggered()), this, SLOT(remove()) );
	connect( actionRemoveAll, SIGNAL(triggered()), this, SLOT(remove()) );

	DNSCacheModel* model =  new DNSCacheModel( m_dns );
	model->setParent( container );
	view->setModel( model );

}
/*----------------------------------------------------------------------------*/
void DNSOptions::remove( bool all )
{
}
