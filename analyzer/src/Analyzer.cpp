#include <stdexcept>
#include "errors.h"
#include "Analyzer.h"
#include "AnalyzeDialog.h"
#include "IPlugin.h"
#include "MainWindow.h"

#define DEFAULT_SNIFFER "./libNetDump.so"
#define SNIFFER_KEY "snifferPlugin"
#define ARRAY_SIZE(array) (sizeof(array) / sizeof(array[0]))

Analyzer::Analyzer(int& argc, char** argv):
	QApplication( argc, argv ),
	autoDeath( false ),
	deviceList( NULL ),
	activeDevice( NULL )
//	sorters(&connections, &packets)
{
	QIcon icon;
	static const int sizes[] = { 16, 32, 48 };
	for (uint i = 0; i < ARRAY_SIZE(sizes); ++i) {
		icon.addPixmap(QString(":/icons/icon-%1.png").arg(sizes[i]));
	}
	setWindowIcon(icon);

	window = new MainWindow();
	if (!window)
		throw std::runtime_error(ERR_MAINWIN_CREATION);
	connect( this, SIGNAL(aboutToQuit()), window, SLOT(deleteLater()) );
	
	qDebug() << "Window attached...";
	
	connect(&recognizers, SIGNAL(error(QString)), this, SIGNAL(error(QString)));
	connect(&recognizers, SIGNAL(addDnsRecord(QHostAddress, QString)), this, SLOT(addDnsRecord(QHostAddress, QString)));
	connect(&recognizers, SIGNAL(recognizerAdded(IRecognizer*)), this, SIGNAL(recognizerAdded(IRecognizer *)));
//	connect(&sorters, SIGNAL(connection(Connection*)), this, SLOT(addConnection(Connection*)), Qt::DirectConnection);
	//connect(&sorters, SIGNAL(connection(Connection *)), &updater, SLOT(takeConnection(Connection *)), Qt::DirectConnection);

	loadSettings();

	sorters.addThreads(1); // just a tip 2 should be fine
	updater.start();

	//recognizers.start();
}
/*----------------------------------------------------------------------------*/
Analyzer::~Analyzer() {
	delete activeDevice;
	delete deviceList;
//	delete snifferPlugin;
}
/*----------------------------------------------------------------------------*/
bool Analyzer::loadPlugin( QString file )
{
	if ( file.isEmpty() ) //nothing to load
		return false;

	qDebug() << "Loading plugin: " << file;

	QPluginLoader * loader = new QPluginLoader( file );
	Q_ASSERT (loader);
	if (loader->isLoaded()) {
		error( "Plugin already loaded!!" );
		loader->unload();
		delete loader;
		return false;
	}
	qDebug() << "New plugin is: " << (loader->isLoaded() ? "LOADED" : "UNLOADED");
	
	QObject * obj = loader->instance();
	IPlugin * plugin = qobject_cast<IPlugin *>( obj );

	// failure
	if (!plugin) {
		qDebug() << "Plugin instantiation" << plugin <<  "failed and it was: " << 
			(loader->isLoaded() ? "LOADED" : "UNLOADED") ;
		qDebug() << obj << loader << loader->errorString();
		loader->unload();
		delete loader;
		return false;
	}
	
	plugins.append( loader );


	plugin->init();

	qDebug() << "Plugin initialized " << obj;

	return true;
}
/*----------------------------------------------------------------------------*/
void Analyzer::addPacket(IDevice * device, QByteArray data){
	// need to check the device, otherwise it could be connected directly
	if (activeDevice == device) 
		sorters.addPacket(data);
}
/*----------------------------------------------------------------------------*/
void Analyzer::addConnection(Connection * conn){
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
bool Analyzer::setAutoPurge(bool on){
	autoDeath = on;
	emit sendAutoPurge(autoDeath);
	return autoDeath == on;
}
/*----------------------------------------------------------------------------*/
void Analyzer::purge() {

}
/*----------------------------------------------------------------------------*/
bool Analyzer::selectDevice(int num){
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
	dnsCache.insert(addr, entry);
	qDebug() << "Added to cache " << addr << " " << name;
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
void Analyzer::loadSettings() {
	QSettings settings(QSettings::UserScope, COMPANY, NAME);
	QString filename = settings.value("snifferPlugin", DEFAULT_SNIFFER).toString();
//	if (QFile::exists(filename))
//		loadSnifferPlugin(filename);
	int size = settings.beginReadArray("recognizers");
	for (int i = 0; i < size; ++i) {
		settings.setArrayIndex(i);
		QString recognizer = settings.value("path").toString();
		bool loaded = settings.value("loaded").toBool();
		recognizers.addRecognizer(recognizer);
		if (!loaded)
			qDebug() << "Should not be loaded..";
	}
	settings.endArray();
}
/*----------------------------------------------------------------------------*/
void Analyzer::saveSettings() {
	qDebug() << "Saving settings";
	int max = recognizers.count();
	QSettings settings(QSettings::UserScope, COMPANY, NAME);
	settings.beginWriteArray("recognizers");
	for (int i = 0;i < max; ++i){
		settings.setArrayIndex(i);
		settings.setValue("path", recognizers[i]->fileName());
		settings.setValue("loaded", recognizers[i]->isLoaded());
	}
	settings.endArray();
}
