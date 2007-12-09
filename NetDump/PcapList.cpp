#include "PcapList.h"


Q_EXPORT_PLUGIN2(netDump, PcapList)

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
//		qCritical()<< "Error in pcap_findalldevs: " << errbuf << "\n";
		return;
	}

	for(d=alldevs; d; d=d->next)
		list.append(NULL);

	pcap_freealldevs(alldevs);
}
/*-------------------------------------------------------------------------------*/
int PcapList::getCount(){
	return list.count();
}
/*-------------------------------------------------------------------------------*/
IDevice * PcapList::operator[](uint num){
	if (num < (uint)list.count()){
	//	qDebug() << "Selected int number: "<< num << endl;
		
		if (list[num] == NULL){
				pcap_if_t *alldevs;
				
				char errbuf[PCAP_ERRBUF_SIZE];
					/* Retrieve the device list */ 
				if(pcap_findalldevs(&alldevs, errbuf) == -1)
				{
					//qCritical()<< "Error in pcap_findalldevs: " << errbuf << "\n";
					return NULL;
				}
				
				pcap_if_t **d;
				uint i = 0;
				
				for(d = &alldevs; (*d); (*d) = (*d)->next)
					if ( i++ == num ){
						
						pcap_freealldevs((*d)->next);
						(*d)->next = NULL;
						
						PcapDev * dev = new PcapDev(*d);
						
						list[num] = dev;
						*d = NULL;
						pcap_freealldevs(alldevs);
						return dev;
					}
					
					
					//qDebug() <<  list[i++]->getDesc() << "\n";
		}
		return list[num];
	}

	return NULL;
}
/*-------------------------------------------------------------------------------*/
QVector<QString> PcapList::getList(){
	QVector<QString> result;
	for(int i = 0;i<list.count();++i)
		result.append((*this)[i]->getDesc());
	return result;

}