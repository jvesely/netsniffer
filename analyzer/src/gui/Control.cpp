#include "Control.h"
#include "uitexts.h"

Control::Control( QWidget * parent, PluginLoader * plugin )
: QWidget( parent ), m_plugin( plugin )
{
	setupUi( this );
	updateStatus( true );
	
	connect( pushButtonBrowse, SIGNAL(clicked()), this, SLOT(getFile()) );
	connect( pushButtonRemove, SIGNAL(clicked()), m_plugin, SLOT(deleteLater()) );
	connect( pushButtonLoad, SIGNAL(clicked()), this, SLOT(switchStatus()) );
	
	connect( m_plugin, SIGNAL(destroyed()), this, SLOT(deleteLater()) );
	connect( m_plugin, SIGNAL(statusChanged( bool)), this, SLOT(updateStatus( bool)) );

	qDebug() << "Created Control: " << this;
}
/*----------------------------------------------------------------------------*/
void Control::switchStatus()
{
	if (m_plugin->isLoaded())
	{
		qDebug() << "Unloading plugin";
		m_plugin->unload();
	} else {
		qDebug() << "Loading plugin";
		m_plugin->load();
	}
}
/*----------------------------------------------------------------------------*/
void Control::getFile()
{
	const QString filename = QFileDialog::getOpenFileName(
		this, tr( UI_PLUGIN_LOAD ), ".", tr( UI_PLUGINS_SUFFIX ) );

	if (!filename.isEmpty())
	{
		labelPath->setText( filename );
		
		if (m_plugin->isLoaded())
			m_plugin->unload();

		m_plugin->setFileName( filename );
		updateStatus( false );
	}
}
/*----------------------------------------------------------------------------*/
void Control::updateStatus( bool loaded )
{
	QString filename = m_plugin->fileName();

	labelPath->setText( QFileInfo( filename ).baseName() );
	labelPath->setToolTip( filename );

	if (m_plugin->isLoaded())
	{
		pushButtonLoad->setText( "&Unload" );
		pushButtonLoad->setIcon( QIcon( ":/control/unload.png" ) );
		labelStatus->setPixmap( QPixmap( ":/control/ok.png" ) );
	} else {
		pushButtonLoad->setText( "&Load" );
		pushButtonLoad->setIcon( QIcon( ":/control/load.png" ) );
		labelStatus->setPixmap( QPixmap( ":/control/error.png" ) );
	}
}
