#pragma once
#include "IDevice.h"

class PcapDevice:private QThread, public IDevice
{
public:
	PcapDevice( pcap_if_t* dev );
	~PcapDevice();

	inline const QString getName() const { return mName; };
	inline const QString getDescription() const { return mDesc; };

	bool captureStart();
	bool captureStop();

	void packet( pcap_pkthdr header, const u_char* data );
	static QString translateName( const QString& name );

private:
	pcap_t* mHandle;
	QString mError;
	const QString mPcapName;
	const QString mName;
	const QString mDesc;
	int mType;
	volatile bool mCapturing;

	pcap_t* open();
	void close();
	void run();

	QByteArray link2IP( const char* data, int len );
	QByteArray parseEthernet( const char* data, int len );

	Q_DISABLE_COPY (PcapDevice);

};

