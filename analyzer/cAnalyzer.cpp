#include <QDebug>
#include "cAnalyzer.h"
#include "cPacket.h"

/*----------------------------------------------------------------------------*/
const IDevice * cAnalyzer::getDev()const {
	return dev;
}
/*----------------------------------------------------------------------------*/
void cAnalyzer::setList(IDevList * devlist){
	list = devlist;	
}
/*----------------------------------------------------------------------------*/
void cAnalyzer::analyze(IDevice * dev, QByteArray data){
	cPacket packet(data);
//	packet.parse();
	QString text("Packet on int ");
	text.append(dev->getName());
	text.append(":\n");
	text.append(packet);
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
	qDebug() << list;
	//if (!list || list->getCount() < (num))
	//	return false;
	qDebug() << "Old dev: " << dev;	
	if(dev)
		dev->disconnect();
	qDebug() << "old disconnected";
	dev = (*list)[num];
	qDebug() << "Selected interface " << dev->getName() <<endl;
	return connect(dev, SIGNAL(packetArrived(IDevice*, QByteArray)), this, SLOT(analyze(IDevice*, QByteArray)));

}
