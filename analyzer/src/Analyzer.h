#ifndef _ANALYZER_H_
#define _ANALYZER_H_

#include <QObject>
#include <QByteArray>
#include <QHash>
#include <QPluginLoader>
#include <QApplication>
#include "IDevice.h"
#include "IDevList.h"
#include "Packet.h"
#include "Connection.h"
#include "ConnectionModel.h"

class MainWindow;
class Analyzer:public QApplication{
	Q_OBJECT
private:
	Analyzer(const Analyzer & analyzer);
	MainWindow * window;
	IDevList * list;
	IDevice * dev;
	QPluginLoader snifferPlg;
	QHash<Packet, Connection > connections;

public:
	ConnectionModel * store;
	
	Analyzer(int& argc, char** argv);
	~Analyzer();

signals:
	void analyzed(Connection * con);
	void devsChanged(QStringList ndevs);

public slots:
	bool loadSniffer(QString path);
	void analyze(IDevice * dev, QByteArray data);
	bool selectNIC(int num);
};
#endif

