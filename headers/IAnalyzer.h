#pragma once

#include "IRecognizer.h"
#include "IDevice.h"

class IAnalyzer: public QApplication
{
	Q_OBJECT	
public:
	inline static IAnalyzer * instance() { return static_cast<IAnalyzer *>(qApp); };
	IAnalyzer(int argc, char ** argv):QApplication(argc, argv){};
	virtual ~IAnalyzer(){};
	virtual IDevice * currentDevice() const = 0;
	virtual QAbstractItemModel * model()  = 0;
	virtual const QStringList devices() const = 0;
	virtual const QList<IRecognizer * > currentRecognizers() = 0;

public slots:
	virtual bool selectDevice(int device) = 0;
	virtual void deepAnalyze() = 0;
	virtual bool setAutoPurge(bool on) = 0;
	virtual void purge() = 0;
	virtual bool loadSnifferPlugin(QString path) = 0;
	virtual bool addRecognizerPlugin(QString path) = 0;
	virtual void saveSettings() = 0;

signals:
	void recognizerAdded(IRecognizer * newRec);
	void devicesChanged(QStringList newDevices);
	void deviceChanged(IDevice * newDevice);
	void error(QString text);
};
