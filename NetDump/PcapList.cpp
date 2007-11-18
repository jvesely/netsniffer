#include "PcapList.h"
#include <QDebug>

PcapList::~PcapList(){
	list.clear();
}
/*-------------------------------------------------------------------------------*/
PcapList::PcapList(){
	pcap_if_t *alldevs;
	pcap_if_t *d;
	char errbuf[PCAP_ERRBUF_SIZE];
		/* Retrieve the device list */
	if(pcap_findalldevs(&alldevs, errbuf) == -1)
	{
		qCritical()<< "Error in pcap_findalldevs: " << errbuf << "\n";
		return;
	}

	for(d=alldevs; d; d=d->next)
	{
		PcapDev * dev;
		dev = new PcapDev(d);
		list.append(dev);
		//qDebug() <<  list[i++]->getDesc() << "\n";
	}
}
/*-------------------------------------------------------------------------------*/
int PcapList::getCount(){
	return list.count();
}
/*-------------------------------------------------------------------------------*/
PcapDev * PcapList::getInterface(uint num){
	if (num < (uint)list.count())
		return list[num];
	return NULL;
}
/*-------------------------------------------------------------------------------*/
QVector<QString> PcapList::getList(){
	QVector<QString> result;
	for(int i = 0;i<list.count();++i)
		result.append(list[i]->getDesc());
	return result;

}