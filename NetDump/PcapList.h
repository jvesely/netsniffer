#include "PcapDev.h"
#include <QVector>
#include <QString>

class PcapList{
	
	QVector<PcapDev * > list;

public:
	QVector<QString > getIfNames();
	int getCount();
	~PcapList();
	PcapList();
	PcapDev * getInterface(uint num);
	QVector<QString > getList();

};