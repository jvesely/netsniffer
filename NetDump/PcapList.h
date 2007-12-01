#include "PcapDev.h"
#include "IDevList.h"
#include <QVector>
#include <QString>

class PcapList:public QObject,public IDevList{

	Q_OBJECT
	Q_INTERFACES(IDevList)
	QVector<PcapDev * > list;

public:
	int getCount();
	~PcapList();
	PcapList();
	IDevice * operator[](uint num);
	QVector<QString > getList();
};

 