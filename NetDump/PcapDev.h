#include "pcap.h"
#include <QThread>
#include <QString>
#include "IDevice.h"

class PcapDev:private QThread, public IDevice{
private:
	pcap_t * handle;
	QString error;
	QString name;
	QString desc;
	volatile bool capturing;
	int type;

	pcap_t * open();
	void close();
	void run();
	QByteArray link2IP(const u_char * data, int len);
	QByteArray ether2IP(const u_char * data, int len);

public:
	
	PcapDev(pcap_if_t * dev);
	~PcapDev();
	QString getName()const;
	bool captureStart();
	int captureStop();
	void packet(pcap_pkthdr header, const u_char * data);
};

