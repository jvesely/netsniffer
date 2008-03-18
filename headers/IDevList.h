#ifndef _IDEVLIST_H_
#define _IDEVLIST_H_
#include "IDevice.h"
#include <QVector>
#include <QString>
#include <QtPlugin>

class IDevList:public QObject {
public:
	virtual int getCount() const = 0;
	inline virtual ~IDevList(){}
	virtual IDevice * operator[](uint num) = 0;
	virtual QVector<QString > getList() const = 0;
};

Q_DECLARE_INTERFACE(IDevList,
                     "Network Interface List/1.0") 
#endif

