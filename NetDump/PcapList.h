#include "PcapDev.h"
#include "IDevList.h"
#include <QVector>
#include <QString>

class PcapList:public IDevList{

	Q_OBJECT
	Q_INTERFACES(IDevList)
	pcap_if_t * alldevs;
	uint count;

public:
	int getCount() const;
	~PcapList();
	PcapList();
	IDevice * operator[](uint num);
	QVector<QString > getList() const;
};

