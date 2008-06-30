#include <QDebug>
#include "MainWindow.h"
#include "Analyzer.h"

#define PATH "./libNetDump.so"

Analyzer::Analyzer(int& argc, char** argv):QApplication(argc, argv), list(NULL), dev(NULL) {
	window = new MainWindow();
	if(! window)
		exit (-1);
	window->show();
	
	connect(window, SIGNAL(selectNIC(int)), this, SLOT(selectNIC(int)));
	connect(this, SIGNAL(devsChanged(QStringList)), window, SLOT(setSelector(QStringList)));
	connect(this, SIGNAL(analyzed(Connection *)), window, SLOT(display(Connection*)));
	loadSniffer(window->getPlugin());
	store = new ConnectionModel();

}
/*----------------------------------------------------------------------------*/
Analyzer::~Analyzer() {
	delete list;
	delete dev;
	delete store;
	delete window;
	snifferPlg.unload();
}
/*----------------------------------------------------------------------------*/
bool Analyzer::loadSniffer(QString path) {
	QPluginLoader newPlg(path);
	IDevList * newlist = qobject_cast<IDevList *>(newPlg.instance());
	if (! newlist) 
		return false;
	
	if (list) {
		delete list;
		snifferPlg.unload();
	}
	
	snifferPlg.setFileName(path);
	snifferPlg.load();
	newPlg.unload();
	list = newlist;
	emit devsChanged(list->getList());
	return true;
}
/*----------------------------------------------------------------------------*/
void Analyzer::analyze(IDevice * device, QByteArray data){
	if (! (dev == device)) // not on my active device
		return ;
	Packet packet(data);
	Connection * con = &(connections[packet] << packet);
	if (con)
		emit analyzed(con);
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
