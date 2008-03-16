#ifndef _CANALYZER_H_
#define _CANALYZER_H_

#include <QObject>
#include <QByteArray>
#include "IDevice.h"
#include "IDevList.h"

class cAnalyzer:public QObject{
	Q_OBJECT
private:
	IDevList * list;
	IDevice * dev;
public:
	cAnalyzer(IDevList * devlist = 0):dev(NULL), list(devlist){};
	const IDevice * getDev() const ;
	void setList(IDevList * devlist);

signals:
	void analyzed (QString text);
	void captureStarted ();
	void captureStopped ();

public slots:
	void analyze(IDevice * dev, QByteArray data);
	bool selectNIC(int num);
	void startNIC();
	void stopNIC();
};
#endif

