#ifndef _IDevice_h_
#define _IDevice_h_

#include <QString>
#include <QObject>
#include <QByteArray>

class IDevice:public QObject{
Q_OBJECT;
public:
	inline virtual ~IDevice() {};
	virtual QString getName() const = 0;
	virtual QString getDesc() const = 0;
	virtual bool capture()=0;
	virtual int stop()=0;
signals:
	virtual void packetArrived(IDevice * dev, QByteArray * data);
};
#endif