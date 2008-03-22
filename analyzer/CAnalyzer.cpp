#include <QDebug>
#include "cAnalyzer.h"


cAnalyzer::cAnalyzer(QObject * devlist) {
	list = NULL;
	dev = NULL;
	setList(devlist);
}
/*----------------------------------------------------------------------------*/
const IDevice * cAnalyzer::getDev()const {
	return dev;
}
/*----------------------------------------------------------------------------*/
bool cAnalyzer::setList(QObject * devlist){
	IDevList * attempt = qobject_cast<IDevList *>(devlist);
	if (attempt)
		list = attempt;
	return list;
}
const IDevList * cAnalyzer::getList() const throw() {
	return list;
}
/*----------------------------------------------------------------------------*/
void cAnalyzer::analyze(IDevice * dev, QByteArray data){
	CPacket packet(data);
	QString text;
	text = (connections[packet] << packet).toString();
	//qDebug() << "Keys: " << connections.keys().count() << "Unique: " <<connections.uniqueKeys().count() << "Records " << connections.count() << endl;
	emit analyzed(text);
}
/*----------------------------------------------------------------------------*/
void cAnalyzer::startNIC(){
	if (dev){
		dev->captureStart();
		emit captureStarted();
	}
}
/*----------------------------------------------------------------------------*/
void cAnalyzer::stopNIC(){
	if (dev){
		dev->captureStop();
		emit captureStopped();
	}
}
/*----------------------------------------------------------------------------*/
bool cAnalyzer::selectNIC(int num){
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
	return connect(dev, SIGNAL(packetArrived(IDevice*, QByteArray)), this, SLOT(analyze(IDevice*, QByteArray)));

}
