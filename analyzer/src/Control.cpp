#include "Control.h"
#include "uitexts.h"

Control::Control( QWidget * parent, QPluginLoader * plugin )
: QWidget( parent ), m_plugin( plugin )
{
	setupUi( this );
	updateStatus();
	
	connect( pushButtonBrowse, SIGNAL(clicked()), this, SLOT(getFile()) );
	
//	connect( rec, SIGNAL(statusChanged(IRecognizer *)), this, SLOT(setStatus(IRecognizer *)));
	connect( m_plugin, SIGNAL(destroyed()), this, SLOT(deleteLater()) );
	connect( pushButtonRemove, SIGNAL(clicked()), m_plugin, SLOT(deleteLater()) );
	connect( pushButtonLoad, SIGNAL(clicked()), this, SLOT(switchStatus()) );
//	connect(this, SIGNAL(setFile( QString )), m_plugin, SLOT(setFileName( QString )) );


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
	
	updateStatus();
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
	}
}
/*----------------------------------------------------------------------------*/
void Control::updateStatus() {
	//qDebug() << "setting status" << status;
	QString filename = m_plugin->fileName();

	labelPath->setText( QFileInfo( filename ).baseName() );
	labelPath->setToolTip( filename );

	if (m_plugin->isLoaded())
	{
//		disconnect( pushButtonLoad, 0, 0, 0 );
//		connect( pushButtonLoad, SIGNAL(clicked()), plugin, SLOT(unload()) );
		pushButtonLoad->setText( "&Unload" );
		pushButtonLoad->setIcon( QIcon( ":/control/unload.png" ) );
		labelStatus->setPixmap( QPixmap( ":/control/ok.png" ) );
	} else {
//		disconnect( pushButtonLoad, 0, 0, 0 );
//		connect(pushButtonLoad, SIGNAL(clicked()), plugin, SLOT(load()));
		pushButtonLoad->setText( "&Load" );
		pushButtonLoad->setIcon( QIcon( ":/control/load.png" ) );
		labelStatus->setPixmap( QPixmap( ":/control/error.png" ) );
	}
}
