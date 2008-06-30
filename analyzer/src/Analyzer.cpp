#include <QDebug>
#include "MainWindow.h"
#include "Analyzer.h"


Analyzer::Analyzer(MainWindow * par):parent(par), list(NULL), dev(NULL) {
	store = new ConnectionModel();
}
/*----------------------------------------------------------------------------*/
Analyzer::~Analyzer() {
	delete list;
	delete dev;
	delete store;
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
void Analyzer::analyze(IDevice * dev, QByteArray data){
	Packet packet(data);
	Connection * con = &(connections[packet] << packet);
	if (con && con->packetCount() == 1)
		store->insertConnection(con);
	else
		store->changeConnection(con);
}
/*----------------------------------------------------------------------------*/
void Analyzer::startNIC(){
	if (dev)
		dev->captureStart();
}
/*----------------------------------------------------------------------------*/
void Analyzer::stopNIC(){
	if (dev)
		dev->captureStop();
}
/*----------------------------------------------------------------------------*/
void Analyzer::devStarted(QString name){
	emit started();
}
void Analyzer::devStopped(QString name){
	emit stopped();
}
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
	connect(dev, SIGNAL(captureStarted(QString)), parent, SLOT(started(QString)));
	connect(dev, SIGNAL(captureStopped(QString)), parent, SLOT(stopped(QString)));
	return connect(dev, SIGNAL(packetArrived(IDevice*, QByteArray)), this, SLOT(analyze(IDevice*, QByteArray)));
}
