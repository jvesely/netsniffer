#ifndef _IDevice_h_
#define _IDevice_h_

#include <QString>
#include <QObject>
#include <QByteArray>

class IDevice:public QObject{
	Q_OBJECT
public:
	inline virtual ~IDevice() {};
	virtual QString getName() const = 0;
	virtual bool captureStart()=0;
	virtual int captureStop()=0;
signals:
	virtual void packetArrived(IDevice * dev, QByteArray data);
	virtual void captureStarted();
	virtual void captureStopped();
};
#endif

