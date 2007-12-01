#include <QDebug>
#include "cAnalyzer.h"


void cAnalyzer::analyze(IDevice * dev, QByteArray * data){
	qDebug() << "Packet on int " << dev->getDesc()<< data->toHex() <<endl;
	delete data;
}