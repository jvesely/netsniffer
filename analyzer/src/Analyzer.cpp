#include <QDebug>
#include <stdexcept>
#include "errors.h"
#include "Analyzer.h"
#include "AnalyzeDialog.h"

#define PATH "./libNetDump.so"

Analyzer::Analyzer(int& argc, char** argv):QApplication(argc, argv), autoDeath(false), deviceList(NULL), activeDevice(NULL), snifferPlugin(NULL)  {
	window = new MainWindow();
	if (!window)
		throw std::runtime_error(ERR_MAINWIN_CREATION);
	window->setModel(&model);
	window->show();
	connect(window, SIGNAL(showOptions()), this, SLOT(showOptions()));
	connect(window, SIGNAL(selectNIC(int)), this, SLOT(selectDevice(int)));
	connect(window, SIGNAL(newSniffer()), this, SLOT(loadSniffer()));
	connect(window, SIGNAL(autoPurge(bool)), this, SLOT(setAutoPurge(bool)));
	connect(window, SIGNAL(startDeepAnalysis()), this, SLOT(deepAnalyze()));
	connect(this, SIGNAL(devsChanged(QStringList)), window, SLOT(setSelector(QStringList)));
	connect(&recognizers, SIGNAL(error(QString)), this, SLOT(error(QString)));
	connect(&recognizers, SIGNAL(addDnsRecord(QHostAddress, QString)), this, SLOT(addDnsRecord(QHostAddress, QString)));
	if (QFile::exists(PATH))
		loadSniffer(PATH); // try default path
	else
		window->printError(ERR_NO_SNIFFER);

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
bool Analyzer::loadSniffer(QString path) {
	if (path.isEmpty())
		path = window->getPlugin();
	if (path.isEmpty()) // user closed did not enter path
		return false;
	qDebug() << "testing new plugin "<< path;
	QPluginLoader *  newPlg = new QPluginLoader(path);
	QObject * inst = newPlg->instance();
	IDevList *  newlist = qobject_cast<IDevList *>(inst);


	if (! newlist ){ // bad plugin 
		qDebug() << "Invalid Plugin" << newlist;
		window->printError(path + "\n" + ERR_INVALID_SNIFFER);
		delete inst; // whatever it is
		delete newPlg;
		return false;
	}
	if (newlist == deviceList) { // same plugin
		delete newPlg;
		window->printError(QString(ERR_LOADED_PLUGIN).arg(path));
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
	
	emit devsChanged(deviceList->getList());
	return true;
}
/*----------------------------------------------------------------------------*/
void Analyzer::analyze(IDevice * device, QByteArray data){
	Q_ASSERT(activeDevice == device); // it should only coma from my active device
	//parse packet
	Packet packet(data); 
	Connection * con(NULL);
	if (connections.contains(packet)){
		con = connections[packet];
	}else{
		con = new Connection(&dnsCache, autoDeath);
		connect(window, SIGNAL(purge()), con, SLOT(purge()));	
		connect(window, SIGNAL(autoPurge(bool)), con, SLOT(setAutoPurge(bool)));
		connections.insert(packet, con);
		model.insertConnection(con);
	}
	(*con) << packet;
	recognizers.process(con);
//	emit analyzed(con);*/
}
/*----------------------------------------------------------------------------*/
bool Analyzer::setAutoPurge(bool on){
	return autoDeath = on;
}
/*----------------------------------------------------------------------------*/
bool Analyzer::selectDevice(int num){
	qDebug() << "Select started" ;	
	if (!deviceList) // nothing to select from
		return false;
	
	//qDebug() << list;
	//qDebug() << "Old dev: " << dev;	
	delete activeDevice; // it is always replaced
	//qDebug() << "old disconnected";
	if (!(activeDevice = (*deviceList)[num]))
		return false; // ohh ohh, something went wrong
	
	qDebug() << "Selected interface " << activeDevice->getName();
	connect(window, SIGNAL(startNIC()), activeDevice, SLOT(captureStart()));
	connect(window, SIGNAL(stopNIC()), activeDevice, SLOT(captureStop()));
	connect(activeDevice, SIGNAL(captureStarted(QString)), window, SLOT(started(QString)));
	connect(activeDevice, SIGNAL(captureStopped(QString)), window, SLOT(stopped(QString)));
	connect(activeDevice, SIGNAL(packetArrived(IDevice*, QByteArray)), this, SLOT(analyze(IDevice*, QByteArray)));
	return true;
}
/*----------------------------------------------------------------------------*/
void Analyzer::showOptions(){

	OptionsDialog opt(window);
	connect(&opt, SIGNAL(newModule(QString)), &recognizers, SLOT(addRecognizer(QString)));
	connect(&opt, SIGNAL(discardModules()), &recognizers, SLOT(dropAll()));
	connect(&recognizers, SIGNAL(recognizerAdded(Recognizer *)), &opt, SLOT(addControl(Recognizer *)));
	int recs = recognizers.count();
	for (int i = 0; i < recs; ++i)
		opt.addControl(recognizers[i]);
	opt.exec();
}
void Analyzer::addDnsRecord(QHostAddress addr, QString name){
	QString * entry = new QString(name);
	dnsCache.insert(addr, entry);
	qDebug() << "Added to cache " << addr << " " << name;
}
void Analyzer::error(QString text){
	if (window)
		window->printError(text);
}

void Analyzer::deepAnalyze() {
	AnalyzeDialog dialog;
	//IConnection * conn = 
	int ret = dialog.exec();
}
