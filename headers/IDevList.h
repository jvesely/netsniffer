#ifndef _IDEVLIST_H_
#define _IDEVLIST_H_
#include "IDevice.h"
#include <QVector>
#include <QString>
#include <QStringList>
#include <QtPlugin>

class IDevList:public QObject {
public:
	virtual uint getCount() const = 0;
	inline virtual ~IDevList(){};
	virtual IDevice * operator[](uint num) const = 0;
	virtual const QStringList getList() const = 0;
};

Q_DECLARE_INTERFACE(IDevList,
                     "Network Interface List/1.0") 
#endif

