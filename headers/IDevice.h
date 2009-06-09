#pragma once

class IDevice:public QObject
{
	Q_OBJECT;

public:
	virtual inline ~IDevice() {};
	virtual const QString getName() const = 0;

public slots:
	virtual bool captureStart() = 0;
	virtual bool captureStop()  = 0;

signals:
	void packetArrived( IDevice * me, QByteArray data );
	void captureStarted( IDevice * me );
	void captureStopped( IDevice * me );
};
