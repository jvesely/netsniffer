#pragma once
#include "IDevice.h"

class PcapDevice:private QThread, public IDevice
{
public:
	PcapDevice( pcap_if_t* dev );
	~PcapDevice();

	inline const QString getName() const { return name; };
	inline const QString getDescription() const { return desc; };

	bool captureStart();
	bool captureStop();

	void packet( pcap_pkthdr header, const u_char* data );
	static QString translateName( const QString& name );

private:
	pcap_t* handle;
	QString error;
	const QString pcapName;
	const QString name;
	const QString desc;
	int type;
	volatile bool capturing;

	pcap_t* open();
	void close();
	void run();

	QByteArray link2IP( const u_char* data, int len );
	QByteArray ether2IP( const u_char* data, int len );

	Q_DISABLE_COPY (PcapDevice);

};

