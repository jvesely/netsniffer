#pragma once
#include "IDevice.h"

class PcapDevice:private QThread, public IDevice
{
public:
	PcapDevice(pcap_if_t * dev);
	~PcapDevice();
	inline const QString getName() const
		{ return name; };
	bool captureStart();
	bool captureStop();
	void packet( pcap_pkthdr header, const u_char * data );

private:
	pcap_t * handle;
	QString error;
	QString name;
	QString desc;
	int type;
	volatile bool capturing;

	pcap_t * open();
	void close();
	void run();
	QByteArray link2IP( const u_char * data, int len );
	QByteArray ether2IP( const u_char * data, int len );

};

