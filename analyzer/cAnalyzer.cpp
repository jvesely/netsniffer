#include <QDebug>
#include "cAnalyzer.h"
#include "cPacket.h"



/*-------------------------------------------------------------------------------*/
const IDevice * cAnalyzer::getDev()const {
	return dev;
}
/*-------------------------------------------------------------------------------*/
void cAnalyzer::setList(IDevList * devlist){
	list = devlist;	
}
/*-------------------------------------------------------------------------------*/
void cAnalyzer::analyze(IDevice * dev, QByteArray * data){
	cPacket packet(*data);
	packet.parse();
	QString text("Packet on int ");
	text.append(dev->getDesc());
	text.append(":\n");
	text.append(packet);
	emit analyzed(text);
	delete data;
}
/*-------------------------------------------------------------------------------*/
void cAnalyzer::startNIC(){
	if (dev){
		dev->capture();
		emit captureStarted();
	}
}
/*-------------------------------------------------------------------------------*/
void cAnalyzer::stopNIC(){
	if (dev){
		dev->stop();
		emit captureStopped();
	}
}
/*-------------------------------------------------------------------------------*/
bool cAnalyzer::selectNIC(int num){

	if (!list||list->getCount()<(num))
		return false;
	if(dev)
		dev->disconnect();

	dev = (*list)[num];
	qDebug() << "Selected interface " << dev->getDesc() <<endl;
	return connect(dev, SIGNAL(packetArrived(IDevice*, QByteArray *)), this, SLOT(analyze(IDevice*, QByteArray *)));

}