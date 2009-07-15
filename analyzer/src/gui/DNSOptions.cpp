/*!
 * @file
 * @brief Short description.
 *
 * Long description. I would paste some Loren Ipsum rubbish here, but I'm afraid
 * It would stay that way. Not that this comment is by any means ingenious but
 * at least people can understand it.
 */
#include "DNSOptions.h"
#include "uitexts.h"

const QString DNSOptions::MY_NAME( "DNS Cache" );

bool DNSOptions::deploy( QWidget* container )
{
	Q_ASSERT (mDns);
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
	
	Q_ASSERT( !mModel );
	mModel =  new DNSCacheModel( mDns );
	Q_ASSERT( mModel );
	mModel->setParent( container );
	
	Q_ASSERT( view );
	view->setModel( mModel );

	connect( mDns, SIGNAL(newEntry( const QHostAddress&, const QString& )),
		this, SLOT(refreshIndicator()) );

//	connect( this, SIGNAL(cacheMax( int )), indicator, SLOT(setMaximum( int )) );
	connect( this, SIGNAL(cacheCount( int )), indicator, SLOT(setValue( int )) );
	indicator->setMaximum( mDns->maxEntries() );
	refreshIndicator();

	capacitySelector->setValue( mDns->maxEntries() );
	connect( capacitySelector, SIGNAL(valueChanged( int )),
		mDns, SLOT(setMaxEntries( int )) );
	connect( mDns, SIGNAL(maxEntries( int )), capacitySelector, SLOT(setValue( int )) );
	connect( mDns, SIGNAL(maxEntries( int )), indicator, SLOT(setMaximum( int )));
	connect( mDns, SIGNAL(maxEntries( int )), indicator, SLOT(repaint()) );

	return true;
}
/*----------------------------------------------------------------------------*/
void DNSOptions::refreshIndicator()
{
	if (mDns) 
	{
//		qDebug() << "emiting cahcen max:" <<  mDns->maxEntries();
//		emit cacheMax( mDns->maxEntries() );
		emit cacheCount( mDns->countEntries() );
	}
}
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
void DNSOptions::remove( int all )
{
	Q_ASSERT (mModel);
	if (!mModel->rowCount())
		return;
	const int reply = QMessageBox::warning(
		NULL, QString( tr( "Options - %1" ) ).arg(MY_NAME),
		all ? tr( UiTexts::DNSCACHE_CLEAR ) : tr( UiTexts::DNSCACHE_REMOVE ), QMessageBox::Yes | QMessageBox::No );

	if ( reply == QMessageBox::Yes )
	{
		if (all)
			view->selectAll();
		Q_ASSERT (view->selectionModel());
		mModel->remove( view->selectionModel()->selectedRows() );
	}
	refreshIndicator();
}
