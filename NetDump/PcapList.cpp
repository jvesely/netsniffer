#include "PcapList.h"


Q_EXPORT_PLUGIN2(netDump, PcapList)

PcapList::~PcapList(){
	pcap_freealldevs(alldevs);
}
/*----------------------------------------------------------------------------*/
PcapList::PcapList(){
	alldevs = NULL;
	char errbuf[PCAP_ERRBUF_SIZE];
		/* Retrieve the device list */
	if(pcap_findalldevs(&alldevs, errbuf) == -1)
	{
		//throw exception here
		return;
	}
}
/*----------------------------------------------------------------------------*/
int PcapList::getCount(){
	int i = 0;
	pcap_if_t * d;
	for(d = alldevs; d; d = d->next)
		++i;
	return i;
}
/*----------------------------------------------------------------------------*/
IDevice * PcapList::operator[](uint num){
	if (num >= getCount())
		//throw exception here
		return NULL;
				
		pcap_if_t **d;
		uint i = 0;
				
		for(d = &alldevs; (*d); (*d) = (*d)->next)
			if ( i++ == num ){
				PcapDev * dev = new PcapDev(*d);
				return dev;
			}
	return NULL;					

}
/*----------------------------------------------------------------------------*/
QVector<QString> PcapList::getList(){
	QVector<QString> result;
	pcap_if_t * dev;
	for(dev = alldevs;dev;dev = dev->next)
		result.append(dev->name);
	return result;
}

