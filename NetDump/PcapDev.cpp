#include "PcapDev.h"
#include <QDateTime>
#include <QDebug>

PcapDev::PcapDev(pcap_if_t *dev):handle(0),name(dev->name),\
desc(dev->description){
	capturing = false;
}
/*----------------------------------------------------------------------------*/
PcapDev::~PcapDev(){
	close();
}
/*----------------------------------------------------------------------------*/
pcap_t * PcapDev::open(){
	char * err = 0;
	if(!handle)
		handle = pcap_open_live(name.toStdString().c_str(), 65536, 1, 100, err);
	if (!handle)
		qDebug() << err;
	return handle;
}
/*----------------------------------------------------------------------------*/
void PcapDev::close(){
	if(handle)
		pcap_close(handle);
	handle = 0;
}
/*----------------------------------------------------------------------------*/
QString PcapDev::getName()const{
	return name;
}
/*----------------------------------------------------------------------------*/
void PcapDev::run(){
	pcap_pkthdr header;
	const u_char * data;
	capturing = true;
	qDebug() << "started capture";
	while (capturing && (data = pcap_next(handle,&header)))
		packet(header,data);
	qDebug() << "ended capture";
}
/*----------------------------------------------------------------------------*/
bool PcapDev::captureStart(){
	if (capturing || !open())
		return false;
	start();
	return true;
}
/*----------------------------------------------------------------------------*/
int PcapDev::captureStop(){
	capturing = false;
	wait();
	close();
	return capturing == false;
}
/*----------------------------------------------------------------------------*/
void PcapDev::packet(pcap_pkthdr header, const u_char * data){
	QByteArray * bytes = new QByteArray((char*)data, header.len);
	emit packetArrived(this, bytes);
}

