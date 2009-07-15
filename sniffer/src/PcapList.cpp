/*!
 * @file
 * @brief Short description.
 *
 * Long description. I would paste some Loren Ipsum rubbish here, but I'm afraid
 * It would stay that way. Not that this comment is by any means ingenious but
 * at least people can understand it.
 */
#include "PcapList.h"
#include "PcapDevice.h"

#ifndef QT_NO_DEBUG
#define DEBUG_TEXT "[ PcapList ]: "
#define PRINT_DEBUG(arg) qDebug() << DEBUG_TEXT << arg
#else
#define PRINT_DEBUG(arg)
#endif

PcapList::~PcapList()
{
	PRINT_DEBUG ("The list is leaving");
	if (mDevices)
		pcap_freealldevs( mDevices );
}
/*----------------------------------------------------------------------------*/
PcapList::PcapList(): mDevices( NULL ), mCount( 0 )
{
	char errbuf[PCAP_ERRBUF_SIZE];

	/* Retrieve the device list */
	if (pcap_findalldevs(&mDevices, errbuf) == -1)
	{
		PRINT_DEBUG ("Device enumeration failed: " << errbuf);
	} else {
		for (pcap_if_t* d = mDevices; d; d = d->next)
			++mCount;
	}
}
/*----------------------------------------------------------------------------*/
IDevice* PcapList::device ( uint num ) const
{
	if (num >= mCount)
		return NULL;
				
	for (pcap_if_t* d = mDevices; d; d = d->next)
	{
		Q_ASSERT (d);
		if ( !num-- ) 
		{
			IDevice* new_device = new PcapDevice( d );
			PRINT_DEBUG ("Returning new device: " << new_device);
			return new_device;
		}
	}
			
	return NULL;					
}
/*----------------------------------------------------------------------------*/
const QStringList PcapList::getNames() const
{
	QStringList result;

	for (pcap_if_t* dev = mDevices; dev; dev = dev->next)
	{
		result << PcapDevice::translateName( dev->name );
	}
	return result;
}

