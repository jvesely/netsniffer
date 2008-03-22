#ifndef _CANALYZER_H_
#define _CANALYZER_H_

#include <QObject>
#include <QByteArray>
#include <QHash>
#include "IDevice.h"
#include "IDevList.h"
#include "CPacket.h"
#include "CConnection.h"

class cAnalyzer:public QObject{
	Q_OBJECT
private:
	IDevList * list;
	IDevice * dev;
	QHash<CPacket, CConnection > connections;

public:
	cAnalyzer(QObject * devlist = 0);
	const IDevice * getDev() const ;
	bool setList(QObject * devlist);
	const IDevList * getList() const throw();

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

