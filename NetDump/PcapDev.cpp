#include "PcapDev.h"
#include <QDebug.h>

PcapDev::PcapDev(pcap_if_t *dev):handle(0),device(*dev){
	device.next = 0;
	capturing = false;
}
/*-------------------------------------------------------------------------------*/
PcapDev::~PcapDev(){
	close();
	pcap_freealldevs(&device);
}
/*-------------------------------------------------------------------------------*/
pcap_t * PcapDev::open(){
	char * err = 0;
	if(!handle)
		handle = pcap_open_live(device.name, 65536, 1, 100, err);
	return handle;
}
/*-------------------------------------------------------------------------------*/
void PcapDev::close(){
	if(handle)
		pcap_close(handle);
}
/*-------------------------------------------------------------------------------*/
QString PcapDev::getName(){
	return device.name;
}
/*-------------------------------------------------------------------------------*/
QString PcapDev::getDesc(){
	return device.description;
}
/*-------------------------------------------------------------------------------*/
void PcapDev::run(){
	//pcap_loop(handle, 0, this->packet, NULL);
	pcap_pkthdr header;
	const u_char * data;
	while (capturing){
		while (data = pcap_next(handle,&header))
			packet(header,data);
		//Sleep(10);
	}
}
/*-------------------------------------------------------------------------------*/
bool PcapDev::capture(){
	open();
	
	if (capturing||!handle)
		return false;
	capturing = true;
	start();
	return true;
}
/*-------------------------------------------------------------------------------*/
int PcapDev::stop(){
	if (capturing)
		pcap_breakloop(handle);
	close();
	return capturing = false;
}
/*-------------------------------------------------------------------------------*/
void PcapDev::packet(pcap_pkthdr header, const u_char * data){
	struct tm *ltime;
	char timestr[16];
	time_t local_tv_sec;
	
	/* convert the timestamp to readable format */
	local_tv_sec = header.ts.tv_sec;
	ltime=localtime(&local_tv_sec);
	strftime( timestr, sizeof timestr, "%H:%M:%S", ltime);
	
	printf("%s,%.6d len:%d\n", timestr, header.ts.tv_usec, header.len);

	emit packetArrived();
}