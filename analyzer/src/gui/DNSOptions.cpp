#include "DNSOptions.h"
#include "uitexts.h"

const QString DNSOptions::MY_NAME( "DNSCache" );

bool DNSOptions::deploy( QWidget* container )
{
	Q_ASSERT (m_dns);
	setupUi( container );
	
	view->addAction( actionRemoveSelected );
	view->addAction( actionRemoveAll );
//	view->setContextMenuPolicy( Qt::ActionsContextMenu );

	QSignalMapper* mapper = new QSignalMapper( container );
	Q_ASSERT (mapper);
	
	connect( actionRemoveSelected, SIGNAL(triggered()), mapper, SLOT(map()) );
	mapper->setMapping( actionRemoveSelected, 0 );
	
	connect( actionRemoveAll, SIGNAL(triggered()), mapper, SLOT(map()) );
	mapper->setMapping( actionRemoveAll, 1 );

	connect( mapper, SIGNAL(mapped( int )), this, SLOT(remove( int )) );
	
	Q_ASSERT( !m_model );
	m_model =  new DNSCacheModel( m_dns );
	Q_ASSERT( m_model );
	m_model->setParent( container );
	
	Q_ASSERT( view );
	view->setModel( m_model );

	connect( m_dns, SIGNAL(newEntry( const QHostAddress&, const QString& )),
		this, SLOT(refreshIndicator()) );

	connect( this, SIGNAL(indicatorMax( int )), indicator, SLOT(setMaximum()) );
	connect( this, SIGNAL(indicatorValue( int )), indicator, SLOT(setValue()) );
	refreshIndicator();


	return true;
}
/*----------------------------------------------------------------------------*/
void DNSOptions::refreshIndicator()
{
	if (m_dns) 
	{
		emit indicatorMax( m_dns->maxEntries() );
		emit indicatorValue( m_dns->countEntries() );
	}
}
/*----------------------------------------------------------------------------*/
void DNSOptions::remove( int all )
{
	if (!m_model->rowCount())
		return;
	const int reply = QMessageBox::warning(
		NULL, tr( UiTexts::REMOVE_ALL ), tr( UiTexts::REMOVE_ALL_EXT ), QMessageBox::Yes | QMessageBox::No );

	if ( reply == QMessageBox::Yes )
	{
		if (all)
			view->selectAll();
		Q_ASSERT (view->selectionModel());
		m_model->remove( view->selectionModel()->selectedRows() );
	}
	indicator->setValue( m_dns->countEntries() );
}
