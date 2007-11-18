#include "PcapDev.h"
#include "IDevList.h"
#include <QVector>
#include <QString>

class PcapList:public IDevList{
	
	QVector<PcapDev * > list;

public:
	int getCount();
	~PcapList();
	PcapList();
	IDevice * getInterface(uint num);
	QVector<QString > getList();

};