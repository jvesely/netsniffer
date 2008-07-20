#ifndef _IANALYZER_H_
#define _IANALYZER_H_

#include <QApplication>
#include <QStringList>
#include <QAbstractItemModel>
#include "IDevice.h"

class IAnalyzer: public QApplication
{
	
	Q_OBJECT

public:
	IAnalyzer(int argc, char ** argv):QApplication(argc, argv){};
	virtual IDevice * currentDevice() const = 0;
	virtual QAbstractItemModel * model()  = 0;
	virtual const QStringList devices() const = 0;

public slots:
	virtual bool selectDevice(int device) = 0;
	virtual void deepAnalyze() = 0;
	virtual bool setAutoPurge(bool on) = 0;
	virtual void purge() = 0;
	virtual bool loadSnifferPlugin(QString path) = 0;
	virtual bool addRecognizerPlugin(QString path) = 0;

signals:
	void devicesChanged(QStringList newDevices);
	void deviceChanged(IDevice * newDevice);
	void error(QString text);
};
#endif
