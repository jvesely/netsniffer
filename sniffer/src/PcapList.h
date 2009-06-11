#pragma once

#include "IDeviceList.h"

class PcapList:public IDeviceList
{
	pcap_if_t* alldevs;
	uint count;

public:
	PcapList();
	~PcapList();
	uint getCount() const;
	IDevice* device( uint num ) const;
	const QStringList getNames() const;
};

