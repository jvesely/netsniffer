#pragma once

#include "IDevice.h"

class IDevList:public QObject {
	Q_OBJECT;
public:
	virtual uint getCount() const = 0;
	inline virtual ~IDevList(){};
	virtual IDevice * operator[](uint num) const = 0;
	virtual const QStringList getList() const = 0;
};

Q_DECLARE_INTERFACE(IDevList,
                     "Network Interface List/1.0") 
