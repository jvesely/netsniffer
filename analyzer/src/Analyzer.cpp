#include <stdexcept>
#include "Analyzer.h"
#include "ConnectionJob.h"
#include "PacketJob.h"
#include "gui/DNSOptions.h"

#define ARRAY_SIZE(array) (sizeof(array) / sizeof(array[0]))

#define DEBUG_TEXT "[ ANALYZER DEBUG ]: "
#include "debug.h"

Analyzer::Analyzer():
	m_autoDeath( false ),
	m_deviceList( NULL ),
	m_activeDevice( NULL )
{

	/* setup options page */	
	QObject::connect(
		&m_pluginOptions, SIGNAL(newPlugin( QString )),
		this, SLOT(addPlugin( QString ))
	);
	QObject::connect(
		this, SIGNAL(newPlugin( PluginLoader* )), 
		&m_pluginOptions, SLOT(addPluginControl( PluginLoader* ))
	);
	
	DNSOptions* dns_opt = new DNSOptions( &m_dnsCache );
	Q_ASSERT (dns_opt);

	/* Add my options pages */
	registerOptionsTab( &m_pluginOptions );
	registerOptionsTab( dns_opt );

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
	if (loader->isLoaded())
	{
		error( "Plugin already loaded!" );
		loader->unload();
		delete loader;
		return false;
	}
	
	const bool success = loader->init();
	
	if (!success)
	{
		error( "Plugin initialization failed!" );
		loader->unload();
		delete loader;
		return false;
	}

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

	Q_UNUSED (check);
	Q_ASSERT (check == 1); // there should have been exactly one instance
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
void Analyzer::addConnection( Connection::Pointer connection )
{
	PRINT_DEBUG << "Added connection " << connection ;
	Q_ASSERT (connection);
	
	m_connections[ connection->networkInfo() ] = connection;
	updater.takeConnection( connection );
	connection->setAutoPurge( m_autoDeath );

	connect( this, SIGNAL(sendAutoPurge( bool )), 
		connection.data(), SLOT(setAutoPurge( bool )) );
	
	connect( connection.data(), SIGNAL(finished( IConnection::Pointer )),
		this, SLOT(removeConnection( IConnection::Pointer )), Qt::DirectConnection );
	connect( connection.data(), SIGNAL(packetArrived( IConnection::Pointer )),
		this, SLOT(packetConnection( IConnection::Pointer )), Qt::DirectConnection );

	emit newConnection( IConnection::Pointer( connection ) );
	packetConnection( connection );
}
/*----------------------------------------------------------------------------*/
void Analyzer::removeConnection( IConnection::Pointer connection )
{
	Q_ASSERT (connection);
	disconnect( connection.data(), 0, this, 0 );
	const int count = m_connections.remove( connection->networkInfo() );

	Q_UNUSED (count)
	Q_ASSERT (count <= 1);
}
/*----------------------------------------------------------------------------*/
void Analyzer::packetConnection( IConnection::Pointer connection )
{
	Q_ASSERT (connection);
	ConnectionJob* job = new ConnectionJob( connection, m_recognizers ); 
	Q_ASSERT(job);
	m_workers.start( job );
}
/*----------------------------------------------------------------------------*/
bool Analyzer::setAutoPurge( bool on )
{
	m_autoDeath = on;
	emit sendAutoPurge( m_autoDeath );
	return m_autoDeath == on;
}
/*----------------------------------------------------------------------------*/
bool Analyzer::selectDevice( int num )
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
bool Analyzer::registerOptionsTab( IOptionsTab* tab )
{
	if (!tab)
		return false;

	m_options.append( tab );
	emit newOptionsTab( tab );
	return true;
}
/*----------------------------------------------------------------------------*/
bool Analyzer::unregisterOptionsTab( IOptionsTab* tab )
{
	return m_options.removeAll( tab );
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
bool Analyzer::registerRecognizer( IRecognizer* recognizer )
{
	PRINT_DEBUG << "Registering recognizer" << recognizer;
	m_recognizers.append( recognizer );
	PRINT_DEBUG << "Registered";
	return true;
}
/*----------------------------------------------------------------------------*/
void Analyzer::unregisterRecognizer( IRecognizer* recognizer )
{
	PRINT_DEBUG << "Removing recognizer.." << recognizer;
	const int count = m_recognizers.removeAll( recognizer );

	Q_UNUSED (count);
	Q_ASSERT (count == 1);
}
/*----------------------------------------------------------------------------*/
void Analyzer::loadSettings()
{
	QSettings settings;
	int size = settings.beginReadArray( "plugins" );
	for (int i = 0; i < size; ++i)
	{
		settings.setArrayIndex(i);
		const QString plugin_file = settings.value( "path" ).toString();
		const bool loaded = settings.value( "loaded" ).toBool();
		if (loaded)
			addPlugin( plugin_file );
		else
			PRINT_DEBUG << "Should be added and not loaded..";
	}
	settings.endArray();
}
/*----------------------------------------------------------------------------*/
void Analyzer::saveSettings()
{
	PRINT_DEBUG << "Saving settings";
	const int max = m_plugins.count();
	QSettings settings;
	settings.beginWriteArray( "plugins" );
	QDir current( QApplication::applicationDirPath() );
	for (int i = 0;i < max; ++i)
	{
		settings.setArrayIndex(i);
		settings.setValue( "path", current.relativeFilePath( m_plugins[i]->fileName() ) );
		settings.setValue( "loaded", m_plugins[i]->isLoaded() );
	}
	settings.endArray();
}
