#include <QDebug>
#include "cAnalyzer.h"


cAnalyzer::cAnalyzer(IDevList * devlist):list(devlist),dev(0){
	qDebug() << list->getCount()<<endl;
	qDebug() << list->getList();
}
/*-------------------------------------------------------------------------------*/
void cAnalyzer::analyze(IDevice * dev, QByteArray * data){
	QString text("Packet on int ");
	text.append(dev->getDesc());
	text.append(":\n");
	text.append(data->toHex());
	emit analyzed(text);
	delete data;
}
/*-------------------------------------------------------------------------------*/
void cAnalyzer::startNIC(){
	if (dev)
		dev->capture();
	qDebug() << "Capturing on "<< dev->getDesc() <<"started\n";
}
/*-------------------------------------------------------------------------------*/
void cAnalyzer::stopNIC(){
	if (dev)
		dev->stop();
	qDebug() << "Capturing on "<< dev->getDesc() <<"stopped\n";
}
/*-------------------------------------------------------------------------------*/
bool cAnalyzer::selectNIC(int num){
	//--num;
	if (!list||list->getCount()<(num))
		return false;
	if(dev)
		dev->disconnect();

	dev = (*list)[num];
	qDebug() << "Selected interface " << dev->getDesc() <<endl;
	return connect(dev, SIGNAL(packetArrived(IDevice*, QByteArray *)), this, SLOT(analyze(IDevice*, QByteArray *)));

}