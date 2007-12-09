#ifndef _CANALYZER_H_
#define _CANALYZER_H_

#include <QObject>
#include <QByteArray>
#include "cPacket.h"
#include "IDevice.h"
#include "IDevList.h"

class cAnalyzer:public QObject{
	Q_OBJECT
public:
	cAnalyzer(IDevList * devlist = 0);

private:
	IDevice * dev;
	IDevList * list;

signals:
	void analyzed (QString text);

public slots:
	void analyze(IDevice * dev, QByteArray * data);
	bool selectNIC(int num);
	void startNIC();
	void stopNIC();
};

#endif