#ifndef _CANALYZER_H_
#define _CANALYZER_H_

#include <QObject>
#include <QByteArray>
#include <QHash>
#include <QPluginLoader>
#include "IDevice.h"
#include "IDevList.h"
#include "CPacket.h"
#include "CConnection.h"
#include "CConnectionModel.h"

class mainWindow;
class CAnalyzer:public QObject{
	Q_OBJECT
private:
	CAnalyzer(const CAnalyzer & analyzer);
	mainWindow * parent;
	IDevList * list;
	IDevice * dev;
	QPluginLoader snifferPlg;
	QHash<CPacket, CConnection > connections;

public:
	CConnectionModel * store;

	CAnalyzer(mainWindow * parent);
	~CAnalyzer();

signals:
	void analyzed (QString text);
	void devsChanged(QStringList ndevs);
	void started();
	void stopped();

public slots:
	bool loadSniffer(QString path);
	void analyze(IDevice * dev, QByteArray data);
	bool selectNIC(int num);
	void startNIC();
	void stopNIC();
	void devStarted(QString name);
	void devStopped(QString name);
};
#endif

