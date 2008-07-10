#include <QDebug>
#include <stdexcept>
#include "errors.h"
#include "Analyzer.h"

#define PATH "./libNetDump.so"

Analyzer::Analyzer(int& argc, char** argv):QApplication(argc, argv), autoDeath(false), list(NULL), dev(NULL), snifferPlg(NULL)  {
	window = new MainWindow();
	if (!window)
		throw std::runtime_error(ERR_MAINWIN_CREATION);
	window->show();
	options = new OptionsDialog(window);

	connect(options, SIGNAL(newModule(QString)), &recognizers, SLOT(addRecognizer(QString)));
	connect(options, SIGNAL(discardModules()), &recognizers, SLOT(dropAll()));
	connect(window, SIGNAL(showOptions()), this, SLOT(showOptions()));
	connect(window, SIGNAL(selectNIC(int)), this, SLOT(selectNIC(int)));
	connect(window, SIGNAL(newSniffer()), this, SLOT(loadSniffer()));
	connect(window, SIGNAL(autoPurge(bool)), this, SLOT(setAutoDeath(bool)));
	connect(this, SIGNAL(devsChanged(QStringList)), window, SLOT(setSelector(QStringList)));
	connect(this, SIGNAL(analyzed(Connection *)), window, SLOT(display(Connection*)));
	connect(&recognizers, SIGNAL(error(QString)), window, SLOT(printError(QString)));
	loadSniffer(PATH); // try default path

}
/*----------------------------------------------------------------------------*/
Analyzer::~Analyzer() {
	delete list;
	delete dev;
	delete window;
//	delete options; deleted by window
	if (snifferPlg && snifferPlg->isLoaded())
		snifferPlg->unload();
	delete snifferPlg;
}
/*----------------------------------------------------------------------------*/
bool Analyzer::loadSniffer(QString path) {
	if (path.isEmpty())
		path = window->getPlugin();
	if (path.isEmpty()) // user closed did not enter path
		return false;
	qDebug() << "testing new plugin "<< path;
	QPluginLoader *  newPlg = new QPluginLoader(path);
	IDevList *  newlist = qobject_cast<IDevList *>(newPlg->instance());
	if (! newlist || list == newlist){ // bad plugin or same plugin
		qDebug() << "Invalid Plugin" << newlist;
		window->printError(path + "\n" + ERR_INVALID_SNIFFER);
		delete newPlg;
		return false;
	}
	qDebug() << "Test OK";	

	// first unload plugin
	delete dev; //QPointer should take care of it :)
	if (list && (list != newlist)) { // test whether the new
		qDebug() << "deleting old list" << list;
		delete list;
	}
	if (snifferPlg && snifferPlg->isLoaded())
		snifferPlg->unload();
	//	delete its loader
	delete snifferPlg;

	//replace
	snifferPlg = newPlg;
	list = newlist;
	
	qDebug() << "new list "<< list->getList();
	
	emit devsChanged(list->getList());
	return true;
}
/*----------------------------------------------------------------------------*/
void Analyzer::analyze(IDevice * device, QByteArray data){
	if (! (dev == device)) // not on my active device, should never happen
		return ;
	Packet packet(data);
	Connection * con(NULL);
	if (connections.contains(packet)){
		con = connections[packet];
	}else{
		con = new Connection(&dns, autoDeath, &recognizers);
		connect(window, SIGNAL(purge()), con, SLOT(purge()));	
		connect(window, SIGNAL(autoPurge(bool)), con, SLOT(setAutoPurge(bool)));
		connect(con, SIGNAL(changed(Connection *)), this, SIGNAL(analyzed(Connection *)));
		connections.insert(packet, con);
		window->display(con, true); // add new connection to roster;
	}
	(*con) << packet;
//	emit analyzed(con);
}
/*----------------------------------------------------------------------------*/
void Analyzer::setAutoDeath(bool on){
	autoDeath = on;
}
/*----------------------------------------------------------------------------*/
bool Analyzer::selectNIC(int num){
	qDebug() << "Select started" ;	
	if (!list)
		return false;
	
	qDebug() << list;
	qDebug() << "Old dev: " << dev;	
	delete dev;
	qDebug() << "old disconnected";
	dev = (*list)[num];
	if (!dev) // ohh ohh, something went wrong
		return false;
	qDebug() << "Selected interface " << dev->getName() <<endl;
	connect(window, SIGNAL(startNIC()), dev, SLOT(captureStart()));
	connect(window, SIGNAL(stopNIC()), dev, SLOT(captureStop()));
	connect(dev, SIGNAL(captureStarted(QString)), window, SLOT(started(QString)));
	connect(dev, SIGNAL(captureStopped(QString)), window, SLOT(stopped(QString)));
	return connect(dev, SIGNAL(packetArrived(IDevice*, QByteArray)), this, SLOT(analyze(IDevice*, QByteArray)));
}

void Analyzer::showOptions(){
	if (options)
		options->exec();
}
