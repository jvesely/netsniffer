#pragma once
#include "IDevice.h"

class PcapDevice:private QThread, public IDevice
{
public:
	PcapDevice( pcap_if_t *dev );
	~PcapDevice();

	const QString getName() const { return mName; };
	const QString getDescription() const { return mDesc; };
	const Stats getStats() const
	{ 
		pcap_stat stats;
		Stats my_stats = {0, 0, 0};

		if (mHandle && (pcap_stats( mHandle, &stats ) != -1))
		{
			my_stats.received = stats.ps_recv;
			my_stats.dropped = stats.ps_drop;
			my_stats.ifdropped = stats.ps_ifdrop;
		}
		return my_stats;
	}

	bool captureStart();
	bool captureStop();

	static QString translateName( const QString &name );

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

	void packet( pcap_pkthdr header, const u_char *data );
	QByteArray link2IP( const char *data, int len );
	QByteArray parseEthernet( QByteArray data );
	QByteArray parseSll( QByteArray data );

	Q_DISABLE_COPY (PcapDevice);
};
