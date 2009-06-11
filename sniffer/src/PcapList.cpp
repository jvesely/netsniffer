#include "PcapList.h"
#include "PcapDevice.h"

#define DEBUG_TEXT "[ PcapList ]: "
#define PRINT_DEBUG qDebug() << DEBUG_TEXT

PcapList::~PcapList()
{
	PRINT_DEBUG << "The list is leaving";
	if (alldevs)
		pcap_freealldevs( alldevs );
}
/*----------------------------------------------------------------------------*/
PcapList::PcapList(): alldevs( NULL )
{
	char errbuf[PCAP_ERRBUF_SIZE];

	/* Retrieve the device list */
	if (pcap_findalldevs(&alldevs, errbuf) == -1)
	{
		//throw exception here
	}
	
}
/*----------------------------------------------------------------------------*/
uint PcapList::getCount() const {
	int i = 0;
	pcap_if_t * d;
	for (d = alldevs; d; d = d->next)
		++i;
	return i;
}
/*----------------------------------------------------------------------------*/
IDevice * PcapList::device ( uint num ) const
{
	if (num >= getCount())
		return NULL;
				
	pcap_if_t * d;
	uint i = 0;
			
	for (d = alldevs; d; d = d->next)
	{
		Q_ASSERT (d);
		if ( i++ == num ) 
		{
			IDevice* new_device = new PcapDevice( d );
			PRINT_DEBUG << "Returning new device: " << new_device;
			return new_device;
		}
	}
			
	return NULL;					
}
/*----------------------------------------------------------------------------*/
const QStringList PcapList::getNames() const
{
	QStringList result;

	for (pcap_if_t* dev = alldevs; dev; dev = dev->next)
	{
		// description is usually better
		if (dev->description)
			result.append( dev->description );
		else
			result.append( dev->name );
	}
	return result;
}

