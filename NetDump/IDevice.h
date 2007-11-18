#ifndef _IDevice_h_
#define _IDevice_h_

#include <QString>
#include <QObject>

class IDevice:public QObject{
Q_OBJECT;
public:
	inline virtual ~IDevice() {};
	virtual QString getName()=0;
	virtual QString getDesc()=0;
	virtual bool capture()=0;
	virtual int stop()=0;
signals:
	virtual void packetArrived();
};
#endif