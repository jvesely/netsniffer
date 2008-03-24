#include <QDebug>
#include "mainWindow.h"
#include "CAnalyzer.h"


CAnalyzer::CAnalyzer(mainWindow * par, QObject * devlist):parent(par) {
	list = NULL;
	dev = NULL;
	setList(devlist);
}
/*----------------------------------------------------------------------------*/
const IDevice * CAnalyzer::getDev()const {
	return dev;
}
/*----------------------------------------------------------------------------*/
bool CAnalyzer::setList(QObject * devlist){
	IDevList * attempt = qobject_cast<IDevList *>(devlist);
	if (attempt) {
		delete list;
		list = attempt;
	}
	return list;
}
/*----------------------------------------------------------------------------*/
const IDevList * CAnalyzer::getList() const throw() {
	return list;
}
/*----------------------------------------------------------------------------*/
void CAnalyzer::analyze(IDevice * dev, QByteArray data){
	CPacket packet(data);
	QString text;
	text = (connections[packet] << packet).toString();
	store.setStringList(store.stringList()<< text);
	//qDebug() << "Keys: " << connections.keys().count() << "Unique: " <<connections.uniqueKeys().count() << "Records " << connections.count() << endl;
	emit analyzed(text);
}
/*----------------------------------------------------------------------------*/
void CAnalyzer::startNIC(){
	if (dev){
		dev->captureStart();
	}
}
/*----------------------------------------------------------------------------*/
void CAnalyzer::stopNIC(){
	if (dev){
		dev->captureStop();
	}
}
/*----------------------------------------------------------------------------*/
bool CAnalyzer::selectNIC(int num){
	qDebug() << "Select started" ;	
	if (!list)
		return false;
	
	qDebug() << list;
	//if (!list || list->getCount() < (num))
	//	return false;
	qDebug() << "Old dev: " << dev;	
	delete dev;
	qDebug() << "old disconnected";
	dev = (*list)[num];

	qDebug() << "Selected interface " << dev->getName() <<endl;
	connect(dev, SIGNAL(captureStarted(QString)), parent, SLOT(started(QString)));
	connect(dev, SIGNAL(captureStopped(QString)), parent, SLOT(stopped(QString)));
	return connect(dev, SIGNAL(packetArrived(IDevice*, QByteArray)), this, SLOT(analyze(IDevice*, QByteArray)));

}
