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

