#pragma once

class IDevice:public QObject{
	Q_OBJECT;
public:
	inline virtual ~IDevice() {};
	virtual QString getName() const = 0;
public slots:
	virtual bool captureStart()=0;
	virtual bool captureStop()=0;
signals:
	virtual void packetArrived(IDevice * dev, QByteArray data);
	virtual void captureStarted(QString);
	virtual void captureStopped(QString);
};
