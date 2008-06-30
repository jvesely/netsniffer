#ifndef _ANALYZER_H_
#define _ANALYZER_H_

#include <QObject>
#include <QByteArray>
#include <QHash>
#include <QPluginLoader>
#include "IDevice.h"
#include "IDevList.h"
#include "Packet.h"
#include "Connection.h"
#include "ConnectionModel.h"

class MainWindow;
class Analyzer:public QObject{
	Q_OBJECT
private:
	Analyzer(const Analyzer & analyzer);
	MainWindow * parent;
	IDevList * list;
	IDevice * dev;
	QPluginLoader snifferPlg;
	QHash<Packet, Connection > connections;

public:
	ConnectionModel * store;

	Analyzer(MainWindow * parent);
	~Analyzer();

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

