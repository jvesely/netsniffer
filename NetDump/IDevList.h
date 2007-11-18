#include "IDevice.h"
#include <QVector>
#include <QString>

class IDevList{
public:
	virtual int getCount() = 0;
	inline virtual ~IDevList(){}
	virtual IDevice * getInterface(uint num) = 0;
	virtual QVector<QString > getList() = 0;

};