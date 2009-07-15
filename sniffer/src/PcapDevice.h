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
	void packet( const QByteArray packet );

	static QString translateName( const QString &name );

private:
	pcap_t* mHandle;
	int mType;
	QString mError;
	const QString mPcapName;
	const QString mName;
	const QString mDesc;

	pcap_t* open();
	void run();
private slots:
	void close();

	QByteArray link2IP( const QByteArray data );
	QByteArray parseEthernet( const QByteArray data );
	QByteArray parseSll( QByteArray data );

	Q_DISABLE_COPY (PcapDevice);
};
