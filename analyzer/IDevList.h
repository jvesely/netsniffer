#include "IDevice.h"
#include <QVector>
#include <QString>
#include <QtPlugin>

class IDevList{
public:
	virtual int getCount() = 0;
	inline virtual ~IDevList(){}
	virtual IDevice * operator[](uint num) = 0;
	virtual QVector<QString > getList() = 0;
};

Q_DECLARE_INTERFACE(IDevList,
                     "Network Interface List/1.0") 