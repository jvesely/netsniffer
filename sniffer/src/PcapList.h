/*!
 * @file
 * @brief Short description.
 *
 * Long description. I would paste some Loren Ipsum rubbish here, but I'm afraid
 * It would stay that way. Not that this comment is by any means ingenious but
 * at least people can understand it.
 */
#pragma once

#include "IDeviceList.h"

class PcapList:public IDeviceList
{
	pcap_if_t* mDevices;
	uint mCount;

public:
	PcapList();
	~PcapList();
	uint count() const { return mCount; };
	IDevice* device( uint num ) const;
	const QStringList getNames() const;
};

