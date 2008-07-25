#pragma once

#include "pcap.h"
#include "IDevList.h"

class PcapList:public IDevList{

	Q_OBJECT
	Q_INTERFACES(IDevList)
	pcap_if_t * alldevs;
	uint count;

public:
	uint getCount() const;
	~PcapList();
	PcapList();
	IDevice * operator[](uint num) const;
	const QStringList getList() const;
};

