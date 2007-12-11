#include "pcap.h"
#include <QThread.h>
#include <QString.h>
#include "IDevice.h"

class PcapDev:private QThread, public IDevice{
private:
	pcap_if_t &device;
	pcap_t *handle;
	QString error;
	bool capturing;

	pcap_t * open();
	void close();
	void run();

public:
	
	PcapDev(pcap_if_t * dev);
	~PcapDev();
	QString getName()const;
	QString getDesc()const;
	bool capture();
	int stop();
	void packet(pcap_pkthdr header, const u_char * data);


};