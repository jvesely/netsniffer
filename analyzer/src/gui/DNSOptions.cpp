#include "DNSOptions.h"
#include "uitexts.h"

const QString DNSOptions::MY_NAME( "DNS Cache" );

bool DNSOptions::deploy( QWidget* container )
{
	Q_ASSERT (m_dns);
	setupUi( container );
	
	view->addAction( actionRemoveSelected );
	view->addAction( actionRemoveAll );

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

//	connect( this, SIGNAL(cacheMax( int )), indicator, SLOT(setMaximum( int )) );
	connect( this, SIGNAL(cacheCount( int )), indicator, SLOT(setValue( int )) );
	indicator->setMaximum( m_dns->maxEntries() );
	refreshIndicator();

	capacitySelector->setValue( m_dns->maxEntries() );
	connect( capacitySelector, SIGNAL(valueChanged( int )),
		m_dns, SLOT(setMaxEntries( int )) );
	connect( m_dns, SIGNAL(maxEntries( int )), capacitySelector, SLOT(setValue( int )) );
	connect( m_dns, SIGNAL(maxEntries( int )), indicator, SLOT(setMaximum( int )));
	connect( m_dns, SIGNAL(maxEntries( int )), indicator, SLOT(repaint()) );

	return true;
}
/*----------------------------------------------------------------------------*/
void DNSOptions::refreshIndicator()
{
	if (m_dns) 
	{
//		qDebug() << "emiting cahcen max:" <<  m_dns->maxEntries();
//		emit cacheMax( m_dns->maxEntries() );
		emit cacheCount( m_dns->countEntries() );
	}
}
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
void DNSOptions::remove( int all )
{
	if (!m_model->rowCount())
		return;
	const int reply = QMessageBox::warning(
		NULL, QString( tr( "Options - %1" ) ).arg(MY_NAME),
		all ? tr( UiTexts::DNSCACHE_CLEAR ) : tr( UiTexts::DNSCACHE_REMOVE ), QMessageBox::Yes | QMessageBox::No );

	if ( reply == QMessageBox::Yes )
	{
		if (all)
			view->selectAll();
		Q_ASSERT (view->selectionModel());
		m_model->remove( view->selectionModel()->selectedRows() );
	}
	refreshIndicator();
}
