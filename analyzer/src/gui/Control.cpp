#include "Control.h"
#include "uitexts.h"

#define DEBUG_TEXT "[ Control ]:"
#include "debug.h"

Control::Control( QWidget* parent, PluginLoader* plugin )
: QWidget( parent ), mPlugin( plugin )
{
	setupUi( this );
	const QString filename = mPlugin->fileName();

	labelPath->setText( QFileInfo( filename ).baseName() );
	labelPath->setToolTip( filename );

	updateStatus( plugin->isLoaded() );
	
	connect( pushButtonRemove, SIGNAL(clicked()), mPlugin, SLOT(deleteLater()) );
	connect( pushButtonLoad, SIGNAL(clicked()), this, SLOT(switchStatus()) );
	
	connect( mPlugin, SIGNAL(destroyed()), this, SLOT(deleteLater()) );
	connect( mPlugin, SIGNAL(statusChanged( bool )), this, SLOT(updateStatus( bool)) );

	PRINT_DEBUG ("Created Control: " << this);
}
/*----------------------------------------------------------------------------*/
void Control::switchStatus()
{
	if (mPlugin->isLoaded())
	{
		PRINT_DEBUG ("Unloading plugin");
		mPlugin->unload();
	} else {
		PRINT_DEBUG ("Loading plugin");
		mPlugin->init();
	}
}
/*----------------------------------------------------------------------------*/
void Control::updateStatus( bool loaded )
{
	if (loaded)
	{
		pushButtonLoad->setText( "&Unload" );
		pushButtonLoad->setIcon( QIcon( ":/control/left16.png" ) );
		labelStatus->setPixmap( QPixmap( ":/status/go24.png" ) );
	} else {
		pushButtonLoad->setText( "&Load" );
		pushButtonLoad->setIcon( QIcon( ":/control/right16.png" ) );
		labelStatus->setPixmap( QPixmap( ":/status/nogo24.png" ) );
	}
	PRINT_DEBUG ("Updated status " << this);
}
