#ifndef _CANALYZER_H_
#define _CANALYZER_H_

#include <QObject>
#include <QByteArray>
#include "IDevice.h"
#include "IDevList.h"

class cAnalyzer:public QObject{
	Q_OBJECT
public:
	cAnalyzer(IDevList * devlist = 0):list(devlist),dev(0){};
	const IDevice * getDev() const ;
	void setList(IDevList * devlist);

private:
	IDevice * dev;
	IDevList * list;

signals:
	void analyzed (QString text);
	void captureStarted ();
	void captureStopped ();
	

public slots:
	void analyze(IDevice * dev, QByteArray * data);
	bool selectNIC(int num);
	void startNIC();
	void stopNIC();
};

#endif