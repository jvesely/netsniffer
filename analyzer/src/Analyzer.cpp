#include <stdexcept>
#include "errors.h"
#include "Analyzer.h"
#include "AnalyzeDialog.h"
#include "IPlugin.h"
#include "MainWindow.h"

#define DEFAULT_SNIFFER "./libNetDump.so"
#define SNIFFER_KEY "snifferPlugin"
#define ARRAY_SIZE(array) (sizeof(array) / sizeof(array[0]))

#define DEBUG_TEXT "[ ANALYZER DEBUG ]"
#define PRINT_DEBUG qDebug() << DEBUG_TEXT

Analyzer::Analyzer(int& argc, char** argv):
	QApplication( argc, argv ),
	autoDeath( false ),
	deviceList( NULL ),
	activeDevice( NULL )
//	sorters(&connections, &packets)
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

	window = new MainWindow();
	if (!window)
		throw std::runtime_error( ERR_MAINWIN_CREATION );
	
	PRINT_DEBUG << "Window attached...";
	
	connect(&recognizers, SIGNAL(error(QString)), this, SIGNAL(error(QString)));
//	connect(&recognizers, SIGNAL(addDnsRecord(QHostAddress, QString)), this, SLOT(addDnsRecord(QHostAddress, QString)));
//	connect(&recognizers, SIGNAL(recognizerAdded(IRecognizer*)), this, SIGNAL(recognizerAdded(IRecognizer *)));
//	connect(&sorters, SIGNAL(connection(Connection*)), this, SLOT(addConnection(Connection*)), Qt::DirectConnection);
	//connect(&sorters, SIGNAL(connection(Connection *)), &updater, SLOT(takeConnection(Connection *)), Qt::DirectConnection);

	loadSettings();

	/* Add my Options */
	registerOptionsPage( &m_pluginOptions );
	connect( &m_pluginOptions, SIGNAL(newPlugin( QString )), this, SLOT(addPlugin( QString )) );
	connect( this, SIGNAL(newPlugin( QPluginLoader* )), &m_pluginOptions, SLOT(addPluginControl( QPluginLoader* )) );

	sorters.addThreads(1); // just a tip 2 should be fine
	updater.start();

	//recognizers.start();
}
/*----------------------------------------------------------------------------*/
Analyzer::~Analyzer()
{
	PRINT_DEBUG << "Dying...";
	delete activeDevice;
//	delete deviceList;
}
/*----------------------------------------------------------------------------*/
bool Analyzer::addPlugin( QString file )
{
	if ( file.isEmpty() ) //nothing to load
		return false;

	PRINT_DEBUG << "Loading plugin: " << file;

	QPluginLoader * loader = new QPluginLoader( file );
	Q_ASSERT (loader);
	if (loader->isLoaded()) {
		error( "Plugin already loaded!!" );
		loader->unload();
		delete loader;
		return false;
	}
	PRINT_DEBUG << "New plugin " << loader << " is: " << (loader->isLoaded() ? "LOADED" : "UNLOADED");
	
	QObject * obj = loader->instance();
	IPlugin * plugin = qobject_cast<IPlugin *>( obj );

	// failure
	if (!plugin) {
		PRINT_DEBUG << "Plugin instantiation" << plugin <<  "failed.";
		PRINT_DEBUG << "Plugin was: " << (loader->isLoaded() ? "" : "UN") << "LOAD";
		PRINT_DEBUG << obj << loader << loader->errorString();
		loader->unload();
		delete loader;
		return false;
	}
	
	m_plugins.append( loader );
	connect( loader, SIGNAL(destroyed( QObject* )),
		this, SLOT( removePlugin( QObject*)) );

	plugin->init( this );

	emit newPlugin( loader );

	PRINT_DEBUG << "Plugin initialized " << obj;

	return true;
}
/*----------------------------------------------------------------------------*/
void Analyzer::removePlugin( QObject* obj )
{
	const int check = m_plugins.removeAll( (QPluginLoader*)obj );
	PRINT_DEBUG << m_plugins << check << obj << qobject_cast<QPluginLoader*>(obj);
	Q_ASSERT( check == 1 ); // there should have been exactly one instance
}
/*----------------------------------------------------------------------------*/
void Analyzer::addPacket(IDevice * device, QByteArray data)
{
	// need to check the device, otherwise it could be connected directly
	if (activeDevice == device) 
		sorters.addPacket(data);
}
/*----------------------------------------------------------------------------*/
void Analyzer::addConnection(Connection * conn)
{
	//qDebug() << "Added connection " << conn ;
	//conn->moveToThread(&updater);// updating threadqApp->thread()); // shift them to main thread
	//conn->setAutoPurge(autoDeath);
//	conn->update(&dnsCache);
	//connect (this, SIGNAL(sendAutoPurge(bool)), conn, SLOT(setAutoPurge(bool)));
	//connect (&updater, SIGNAL(update()), conn, SLOT(update())); //const QCache<QHostAddress, QString> * )));
	//model_.insertConnection(conn);
//	recognizers.insertQuick(conn); 
}
/*----------------------------------------------------------------------------*/
bool Analyzer::setAutoPurge(bool on)
{
	autoDeath = on;
	emit sendAutoPurge(autoDeath);
	return autoDeath == on;
}
/*----------------------------------------------------------------------------*/
void Analyzer::purge() {
#warning purge implement
}
/*----------------------------------------------------------------------------*/
bool Analyzer::selectDevice( int num )
{
	if (!deviceList) // nothing to select from
		return false;
	
	delete activeDevice; // it is always replaced
	
	if (!(activeDevice = (*deviceList)[num]))
		return false; // ohh ohh, something went wrong
	
	connect(activeDevice, SIGNAL(packetArrived(IDevice*, QByteArray)), this, SLOT(addPacket(IDevice*, QByteArray)), Qt::DirectConnection);
	
	emit deviceChanged(activeDevice);
	return true;
}
/*----------------------------------------------------------------------------*/
void Analyzer::addDnsRecord(QHostAddress addr, QString name){
	QString * entry = new QString(name);
	m_dnsCache->insert(addr, entry);
	qDebug() << "Added to cache " << addr << " " << name;
}
/*----------------------------------------------------------------------------*/
void Analyzer::registerOptionsPage( IOptionsPage* new_options )
{
	Q_ASSERT (new_options);
	m_options.append( new_options );
	emit newOptionsPage( new_options );
}
/*----------------------------------------------------------------------------*/
bool Analyzer::registerDeviceList( IDeviceList* devices )
{
	deviceList = devices;
	const QStringList new_devices_names = (deviceList ?  deviceList->getNames() : QStringList());
	emit devicesChanged( new_devices_names );
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
//	QString filename = settings.value("snifferPlugin", DEFAULT_SNIFFER).toString();
//	if (QFile::exists(filename))
//		loadSnifferPlugin(filename);
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
	//recognizers.count();
	QSettings settings(QSettings::UserScope, COMPANY, NAME);
	settings.beginWriteArray("plugins");
	for (int i = 0;i < max; ++i){
		settings.setArrayIndex(i);
		settings.setValue("path", m_plugins[i]->fileName());
		settings.setValue("loaded", m_plugins[i]->isLoaded());
	}
	settings.endArray();
}
