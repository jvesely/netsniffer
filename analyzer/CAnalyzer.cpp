#include <QDebug>
#include "mainWindow.h"
#include "CAnalyzer.h"


CAnalyzer::CAnalyzer(mainWindow * par):parent(par), list(NULL), dev(NULL) {
	store = new CConnectionModel();
}
/*----------------------------------------------------------------------------*/
CAnalyzer::~CAnalyzer() {
	delete list;
	delete dev;
	delete store;
	snifferPlg.unload();
}
/*----------------------------------------------------------------------------*/
bool CAnalyzer::loadSniffer(QString path) {
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
void CAnalyzer::analyze(IDevice * dev, QByteArray data){
	CPacket packet(data);
	CConnection * con = &(connections[packet] << packet);
	if (con && con->packetCount() == 1)
		store->insertConnection(con);
	else
		store->changeConnection(con);
}
/*----------------------------------------------------------------------------*/
void CAnalyzer::startNIC(){
	if (dev)
		dev->captureStart();
}
/*----------------------------------------------------------------------------*/
void CAnalyzer::stopNIC(){
	if (dev)
		dev->captureStop();
}
/*----------------------------------------------------------------------------*/
void CAnalyzer::devStarted(QString name){
	emit started();
}
void CAnalyzer::devStopped(QString name){
	emit stopped();
}
bool CAnalyzer::selectNIC(int num){
	qDebug() << "Select started" ;	
	if (!list)
		return false;
	
	qDebug() << list;
	qDebug() << "Old dev: " << dev;	
	delete dev;
	qDebug() << "old disconnected";
	dev = (*list)[num];

	qDebug() << "Selected interface " << dev->getName() <<endl;
	connect(dev, SIGNAL(captureStarted(QString)), parent, SLOT(started(QString)));
	connect(dev, SIGNAL(captureStopped(QString)), parent, SLOT(stopped(QString)));
	return connect(dev, SIGNAL(packetArrived(IDevice*, QByteArray)), this, SLOT(analyze(IDevice*, QByteArray)));
}
