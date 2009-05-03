#include <stdexcept>
#include "Analyzer.h"
#include "errors.h"
#include "PacketJob.h"
#include "gui/MainWindow.h"

#define ARRAY_SIZE(array) (sizeof(array) / sizeof(array[0]))

#define SORTER_THREADS 1

#define DEBUG_TEXT "[ ANALYZER DEBUG ]: "
#include "debug.h"

Analyzer::Analyzer( int& argc, char** argv ):
	QApplication( argc, argv ),
	m_autoDeath( false ),
	m_model( &m_dnsCache ),
	m_deviceList( NULL ),
	m_activeDevice( NULL )
{
	{ /* Creating main window icon. */
		QIcon icon;
		const int sizes[] = { 16, 32, 48 };

		for (uint i = 0; i < ARRAY_SIZE(sizes); ++i) {
			icon.addPixmap(QString(":/icons/icon-%1.png").arg(sizes[i]));
		}
		
		setWindowIcon( icon );
		PRINT_DEBUG << icon;
	}

	m_window = new MainWindow();
	if (!m_window)
		throw std::runtime_error( ERR_MAINWIN_CREATION );
	
	PRINT_DEBUG << "Window attached...";

	connect( &m_dnsCache, SIGNAL(newEntry( const QHostAddress, const QString )),
		&m_model, SLOT( DNSRefresh( const QHostAddress, const QString )) );

	loadSettings();

	/* Add my Options */
	registerOptionsPage( &m_pluginOptions );

	connect( &m_pluginOptions, SIGNAL(newPlugin( QString )), this, SLOT(addPlugin( QString )) );
	connect( this, SIGNAL(newPlugin( PluginLoader* )), &m_pluginOptions, SLOT(addPluginControl( PluginLoader* )) );

	/* Start the Connection keeper */
	updater.start();
}
/*----------------------------------------------------------------------------*/
Analyzer::~Analyzer()
{
	PRINT_DEBUG << "Dying...";
	delete m_activeDevice;
}
/*----------------------------------------------------------------------------*/
bool Analyzer::addPlugin( const QString& file )
{
	if ( file.isEmpty() ) //nothing to load
		return false;

	PRINT_DEBUG << "Loading plugin: " << file;

	PluginLoader * loader = new PluginLoader( file );
	Q_ASSERT (loader);
	if (loader->loaded()) {
		error( "Plugin already loaded!!" );
		loader->unload();
		delete loader;
		return false;
	}
	
	const bool success = loader->init();
	
	if (!success) return false;

	m_plugins.append( loader );
	connect( loader, SIGNAL(destroyed( QObject* )),
		this, SLOT(removePlugin( QObject* )) );

	emit newPlugin( loader );

	return true;
}
/*----------------------------------------------------------------------------*/
void Analyzer::removePlugin( QObject* obj )
{
	const int check = m_plugins.removeAll( (PluginLoader*)obj );
	Q_ASSERT( check == 1 ); // there should have been exactly one instance
}
/*----------------------------------------------------------------------------*/
void Analyzer::addPacket( IDevice* device, QByteArray data )
{
	if (m_activeDevice != device)
		return;
	
	PacketJob* new_job = new PacketJob( data, m_connections );
	Q_ASSERT (new_job);
	m_workers.start( new_job );
	PRINT_DEBUG << "Workers ActiveThreadCount: " << m_workers.activeThreadCount();
}
/*----------------------------------------------------------------------------*/
void Analyzer::addConnection( Connection* connection )
{
	PRINT_DEBUG << "Added connection " << connection ;
	Q_ASSERT (connection);
	updater.takeConnection( connection );
	connection->setAutoPurge( m_autoDeath );

	connect( this, SIGNAL(sendAutoPurge( bool )), 
		connection, SLOT(setAutoPurge( bool )) );
	connect( connection, SIGNAL(finished( Connection* )),
		this, SLOT(removeConnection( Connection* )), Qt::DirectConnection );
	connect( connection, SIGNAL(packetArrived( Connection* )),
		this, SLOT(packetConnection( Connection* )), Qt::DirectConnection );

	m_model.insertConnection( connection );
}
/*----------------------------------------------------------------------------*/
void Analyzer::removeConnection( Connection* connection )
{
	Q_ASSERT (connection);
	const int removed = 
		m_connections.remove( connection->networkInfo() ) *
		m_lastUsedAnalyzers.remove( connection );
	Q_ASSERT (removed <= 1);
}
/*----------------------------------------------------------------------------*/
void Analyzer::packetConnection( Connection* connection )
{
	Q_ASSERT (connection);
	m_model.updateConnection( connection, ConnectionModel::PacketCount );
}
/*----------------------------------------------------------------------------*/
bool Analyzer::setAutoPurge( bool on )
{
	m_autoDeath = on;
	emit sendAutoPurge( m_autoDeath );
	return m_autoDeath == on;
}
/*----------------------------------------------------------------------------*/
bool Analyzer::selectDevice( const int num )
{
	if (!m_deviceList) // nothing to select from
		return false;

	PRINT_DEBUG << "Old device: " << m_activeDevice;
	delete m_activeDevice; // it is always replaced
	
	m_activeDevice = (*m_deviceList)[num];
	PRINT_DEBUG << "Selecting device: " << m_activeDevice << num;

	if (!m_activeDevice)
	{
		error("Selecting device failed");
		return false;
	}

	connect( m_activeDevice, SIGNAL(packetArrived(IDevice*, QByteArray)),
		this, SLOT(addPacket(IDevice*, QByteArray)), Qt::DirectConnection );
	
	emit deviceChanged( m_activeDevice );
	return true;
}
/*----------------------------------------------------------------------------*/
bool Analyzer::registerOptionsPage( IOptionsPage* new_options )
{
	if (!new_options)
		return false;

	m_options.append( new_options );
	emit newOptionsPage( new_options );
	return true;
}
/*----------------------------------------------------------------------------*/
bool Analyzer::registerDeviceList( IDeviceList* devices )
{
	m_deviceList = devices;
	PRINT_DEBUG << "Registering device list: " << devices;
	
	if (m_deviceList) 
	{
		connect( devices, SIGNAL(destroyed()), this, SLOT(registerDeviceList()) );
		PRINT_DEBUG << "Adding new devices: " <<  m_deviceList->getNames();
		emit devicesChanged( m_deviceList->getNames() );
	} else {
		PRINT_DEBUG << "This should erase all offered devices";
		delete m_activeDevice;
		emit devicesChanged( QStringList() );
		PRINT_DEBUG << "Devices erased";
	}

	return true;
}
/*----------------------------------------------------------------------------*/
/*QWidget * Analyzer::deepAnalyze(QModelIndex index) {

	Connection * victim = model_.connection(index);
	qDebug() << "Analyzing" << victim;
	if (! victim) // it might hace died and been removed
		return NULL;
	return ((ARecognizerEngine*)victim->getLast())->analyze(victim);
} // */
/*----------------------------------------------------------------------------*/
void Analyzer::loadSettings()
{
	QSettings settings(QSettings::UserScope, COMPANY, NAME);
	int size = settings.beginReadArray("plugins");
	for (int i = 0; i < size; ++i) {
		settings.setArrayIndex(i);
		QString plugin_file = settings.value("path").toString();
		const bool loaded = settings.value("loaded").toBool();
		if (loaded)
			addPlugin( plugin_file );
		else
			PRINT_DEBUG << "Should added and not loaded..";
	}
	settings.endArray();
}
/*----------------------------------------------------------------------------*/
void Analyzer::saveSettings()
{
	PRINT_DEBUG << "Saving settings";
	const int max = m_plugins.count();
	QSettings settings(QSettings::UserScope, COMPANY, NAME);
	settings.beginWriteArray("plugins");
	for (int i = 0;i < max; ++i){
		settings.setArrayIndex(i);
		settings.setValue("path", m_plugins[i]->fileName());
		settings.setValue("loaded", m_plugins[i]->loaded());
	}
	settings.endArray();
}
