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
	while (capturing){
		while ((data = pcap_next(handle,&header)))
			packet(header,data);
	}
}
/*----------------------------------------------------------------------------*/
bool PcapDev::capture(){
	open();
	if (capturing||!handle)
		return false;
	capturing = true;
	start();
	return true;
}
/*----------------------------------------------------------------------------*/
int PcapDev::stop(){
	if (capturing)
		pcap_breakloop(handle);
	quit();
	close();
	return capturing = false;
}
/*----------------------------------------------------------------------------*/
void PcapDev::packet(pcap_pkthdr header, const u_char * data){
	QByteArray * bytes = new QByteArray((char*)data, header.len);
	emit packetArrived(this, bytes);
}

