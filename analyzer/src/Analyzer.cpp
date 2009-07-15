/*!
 * @file
 * @brief Short description.
 *
 * Long description. I would paste some Loren Ipsum rubbish here, but I'm afraid
 * It would stay that way. Not that this comment is by any means ingenious but
 * at least people can understand it.
 */
#include <stdexcept>
#include "Analyzer.h"
#include "ConnectionJob.h"
#include "PacketJob.h"
#include "gui/DNSOptions.h"

#define ARRAY_SIZE(array) (sizeof(array) / sizeof(array[0]))

#define DEBUG_TEXT "[ ANALYZER DEBUG ]: "
#include "debug.h"

Analyzer::Analyzer():
	mDeviceList( NULL ),
	mActiveDevice( NULL )
{
	qRegisterMetaType<IConnection::Pointer>( "IConnection::Pointer" );
	qRegisterMetaType<IConnection::Status>( "IConnection::Status" );

	/* setup options page */	
	QObject::connect(
		&mPluginOptions, SIGNAL(newPlugin( QString )),
		this, SLOT(addPlugin( QString ))
	);
	QObject::connect(
		this, SIGNAL(newPlugin( PluginLoader* )), 
		&mPluginOptions, SLOT(addPluginControl( PluginLoader* ))
	);
	
	DNSOptions* dns_opt = new DNSOptions( &mDnsCache );
	Q_ASSERT (dns_opt);

	/* Add my options pages */
	registerOptionsTab( &mPluginOptions );
	registerOptionsTab( dns_opt );

	/* Start the Connection keeper */
	mUpdater.start();

	//moveToThread( &mUpdater );
}
/*----------------------------------------------------------------------------*/
Analyzer::~Analyzer()
{
	PRINT_DEBUG ("Dying...");
	delete mActiveDevice;
	mWorkers.waitForDone();
	mUpdater.quit();
	if (!mUpdater.wait( 250 ))
		mUpdater.terminate();
}
/*----------------------------------------------------------------------------*/
bool Analyzer::addPlugin( const QString& file )
{
	if ( file.isEmpty() ) //nothing to load
		return false;

	PRINT_DEBUG ("Loading plugin: " << file);

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

	mPlugins.append( loader );
	connect( loader, SIGNAL(destroyed( QObject* )),
		this, SLOT(removePlugin( QObject* )) );

	emit newPlugin( loader );

	return true;
}
/*----------------------------------------------------------------------------*/
void Analyzer::removePlugin( QObject* obj )
{
	const int check = mPlugins.removeAll( (PluginLoader*)obj );

	Q_UNUSED (check);
	Q_ASSERT (check == 1); // there should have been exactly one instance
}
/*----------------------------------------------------------------------------*/
void Analyzer::addPacket( IDevice* device, QByteArray data )
{
	if (mActiveDevice != device)
		return;
	
	PacketJob* new_job = new PacketJob( data, mConnections );
	Q_ASSERT (new_job);
	mWorkers.start( new_job );
}
/*----------------------------------------------------------------------------*/
bool Analyzer::addConnection( IConnection::Pointer connection )
{
	PRINT_DEBUG ("Added connection " << connection);
	Q_ASSERT (connection);
	
	if (!mConnections.testAndInsert( connection->networkInfo(), connection ))
		return false;

	mUpdater.takeConnection( connection );
	
	connect( connection.data(), SIGNAL(packetArrived( IConnection::Pointer )),
		this, SLOT(packetConnection( IConnection::Pointer )), Qt::DirectConnection );

	emit newConnection( connection );
	packetConnection( connection );
	return true;
}
/*----------------------------------------------------------------------------*/
void Analyzer::removeConnection( IConnection::Pointer connection )
{
	Q_ASSERT (connection);
	disconnect( connection.data(), 0, this, 0 );
	const int count = mConnections.remove( connection->networkInfo() );

	Q_UNUSED (count);
	Q_ASSERT (count <= 1);
}
/*----------------------------------------------------------------------------*/
void Analyzer::packetConnection( IConnection::Pointer connection )
{
	Q_ASSERT (connection);
	ConnectionJob* job = new ConnectionJob( connection, mRecognizers ); 
	Q_ASSERT (job);
	mWorkers.start( job );
}
/*----------------------------------------------------------------------------*/
bool Analyzer::selectDevice( int num )
{
	if (!mDeviceList) // nothing to select from
		return false;

	PRINT_DEBUG ("Old device: " << mActiveDevice);
	delete mActiveDevice; // it is always replaced
	
	mActiveDevice = (*mDeviceList)[num];
	PRINT_DEBUG ("Selecting device: " << mActiveDevice << num);

	if (!mActiveDevice)
	{
		error("Selecting device failed");
		return false;
	}

	connect( mActiveDevice, SIGNAL(packetArrived(IDevice*, QByteArray)),
		this, SLOT(addPacket(IDevice*, QByteArray)), Qt::DirectConnection );
	
	emit deviceChanged( mActiveDevice );
	return true;
}
/*----------------------------------------------------------------------------*/
bool Analyzer::registerOptionsTab( IOptionsTab* tab )
{
	if (!tab)
		return false;

	mOptions.append( tab );
	emit newOptionsTab( tab );
	return true;
}
/*----------------------------------------------------------------------------*/
bool Analyzer::unregisterOptionsTab( IOptionsTab* tab )
{
	return mOptions.removeAll( tab );
}
/*----------------------------------------------------------------------------*/
bool Analyzer::registerDeviceList( IDeviceList* devices )
{
	mDeviceList = devices;
	PRINT_DEBUG ("Registering device list: " << devices);
	
	if (mDeviceList) 
	{
		connect( devices, SIGNAL(destroyed()), this, SLOT(registerDeviceList()) );
		PRINT_DEBUG ("Adding new devices: " << mDeviceList->getNames());
		emit devicesChanged( mDeviceList->getNames() );
	} else {
		PRINT_DEBUG ("This should erase all offered devices");
		delete mActiveDevice;
		emit devicesChanged( QStringList() );
		PRINT_DEBUG ("Devices erased");
	}

	return true;
}
/*----------------------------------------------------------------------------*/
bool Analyzer::registerRecognizer( IRecognizer* recognizer )
{
	PRINT_DEBUG ("Registering recognizer" << recognizer);
	mRecognizers.append( recognizer );
	PRINT_DEBUG ("Registered");
	return true;
}
/*----------------------------------------------------------------------------*/
void Analyzer::unregisterRecognizer( IRecognizer* recognizer )
{
	PRINT_DEBUG ("Removing recognizer.." << recognizer);
	const int count = mRecognizers.removeAll( recognizer );

	Q_UNUSED (count);
	Q_ASSERT (count == 1);
}
/*----------------------------------------------------------------------------*/
void Analyzer::loadSettings()
{
	const QDir app_dir( QApplication::applicationDirPath() );

	QSettings settings;
	int size = settings.beginReadArray( "plugins" );
	for (int i = 0; i < size; ++i)
	{
		settings.setArrayIndex(i);
		const QString plugin_file = app_dir.absoluteFilePath( settings.value( "file" ).toString() );
		const bool loaded = settings.value( "loaded" ).toBool();
		const bool success = addPlugin( plugin_file );
		if (!loaded && success)	{
			mPlugins.back()->unload();
		} 
	}
	settings.endArray();
}
/*----------------------------------------------------------------------------*/
void Analyzer::saveSettings()
{
	PRINT_DEBUG ("Saving settings");
	const int max = mPlugins.count();
	QSettings settings;
	settings.beginWriteArray( "plugins" );
	const QDir app_dir( QApplication::applicationDirPath() );
	for (int i = 0;i < max; ++i)
	{
		settings.setArrayIndex(i);
		settings.setValue( "file", app_dir.relativeFilePath( mPlugins[i]->fileName() ) );
		settings.setValue( "loaded", mPlugins[i]->isLoaded() );
	}
	settings.endArray();
}
