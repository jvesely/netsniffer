#pragma once

class IDevice:public QObject
{
	Q_OBJECT;

public:
	inline virtual ~IDevice() {};
	virtual const QString getName() const = 0;

public slots:
	virtual bool captureStart() = 0;
	virtual bool captureStop()  = 0;

signals:
	virtual void packetArrived( IDevice * , QByteArray );
	virtual void captureStarted( IDevice * );
	virtual void captureStopped( IDevice * );

};
