#include <stdexcept>
#include "errors.h"
#include "Analyzer.h"
#include "AnalyzeDialog.h"

#define DEFAULT_SNIFFER "./libNetDump.so"
#define SNIFFER_KEY "snifferPlugin"
#define ARRAY_SIZE(array) (sizeof(array) / sizeof(array[0]))

Analyzer::Analyzer(int& argc, char** argv):IAnalyzer(argc, argv), autoDeath(false), deviceList(NULL), activeDevice(NULL), snifferPlugin(NULL)  {


	QIcon icon;
	static const int sizes[] = { 16, 32, 48 };
	for (uint i = 0; i < ARRAY_SIZE(sizes); ++i) {
		icon.addPixmap(QString(":/icons/icon-%1.png").arg(sizes[i]));
	}
	setWindowIcon(icon);

	window = new MainWindow(this);
	if (!window)
		throw std::runtime_error(ERR_MAINWIN_CREATION);
//	window->show();
//	window->attach(this);
	qDebug() << "Window attached...";
	connect(&recognizers, SIGNAL(error(QString)), this, SIGNAL(error(QString)));
	connect(&recognizers, SIGNAL(addDnsRecord(QHostAddress, QString)), this, SLOT(addDnsRecord(QHostAddress, QString)));
	connect(&recognizers, SIGNAL(recognizerAdded(IRecognizer*)), this, SIGNAL(recognizerAdded(IRecognizer *)));

	loadSettings();
	sorters.addThreads(2); // just a tip 2 should be fine
	//recognizers.start();
}
/*----------------------------------------------------------------------------*/
Analyzer::~Analyzer() {
	delete activeDevice;
	delete window;
	delete deviceList;
	if (snifferPlugin && snifferPlugin->isLoaded())
		snifferPlugin->unload();
	delete snifferPlugin;
}
/*----------------------------------------------------------------------------*/
bool Analyzer::loadSnifferPlugin(QString path) {
	if (path.isEmpty()) // user closed did not enter path
		return false;
	qDebug() << "testing new plugin "<< path;
	QPluginLoader *  newPlg = new QPluginLoader(path);
	QObject * inst = newPlg->instance();
	IDevList *  newlist = qobject_cast<IDevList *>(inst);


	if (! newlist ){ // bad plugin 
		qDebug() << "Invalid Plugin" << newlist;
		error(QString(ERR_INVALID_SNIFFER).arg(path));
		delete inst; // whatever it is
		delete newPlg;
		return false;
	}
	if (newlist == deviceList) { // same plugin
		delete newPlg;
		error(QString(ERR_LOADED_PLUGIN).arg(path));
		return false;
	}
	qDebug() << "Test OK";	

	// first delete old stuff 
	delete activeDevice; //QPointer should take care of it :)
	delete deviceList;

	//unload
	if (snifferPlugin && snifferPlugin->isLoaded())
		snifferPlugin->unload();
	
	//	delete  loader
	delete snifferPlugin;

	//replace
	snifferPlugin = newPlg;
	deviceList = newlist;
	
	qDebug() << "new list "<< deviceList->getList();
	
	emit devicesChanged(deviceList->getList());
	// save setting
	QSettings settings(QSettings::UserScope, COMPANY, NAME);	
	settings.setValue(SNIFFER_KEY, path);
	return true;
}
/*----------------------------------------------------------------------------*/
void Analyzer::addPacket(IDevice * device, QByteArray data){
	if (activeDevice != device) return; 
	// something went wrong packet from incorect device
	sorters.addPacket(data);
}
void Analyzer::analyze(){
/*	//packetSem.acquire();
//	QMutexLocker lock(&packetGuard);
	if (packets.isEmpty()) return; // something went wrong
	Packet * packet = packets.dequeue();
	if (! packet ) return; //there might have been NULL pointer (should not happen)
	QPointer<Connection>  &con = connections[*packet];
	if ( !con ) { 
		//null (deleted or just constructed)
		con = new Connection(dnsCache, autoDeath, *packet);
		connect (this, SIGNAL(sendAutoPurge(bool)), con, SLOT(setAutoPurge(bool)));
		//connections.insert(packet, con); // if it was there it is replaced
		model_.insertConnection(con);
	} else
		(*con) << *packet;
	delete packet;
	recognizers.insertQuick(con); */
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
	
	connect(activeDevice, SIGNAL(packetArrived(IDevice*, QByteArray)), this, SLOT(addPacket(IDevice*, QByteArray)));
	
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
}
/*----------------------------------------------------------------------------*/
void Analyzer::loadSettings() {
	QSettings settings(QSettings::UserScope, COMPANY, NAME);
	QString filename = settings.value("snifferPlugin", DEFAULT_SNIFFER).toString();
	if (QFile::exists(filename))
		loadSnifferPlugin(filename);
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
