#ifndef _ANALYZER_H_
#define _ANALYZER_H_

#include <QObject>
#include <QByteArray>
#include <QHash>
#include <QCache>
#include <QPluginLoader>
#include <QApplication>
#include "ConnectionModel.h"
#include "IDevice.h"
#include "IDevList.h"
#include "Packet.h"
#include "Connection.h"
#include "RManager.h"
#include "MainWindow.h"
#include "OptionsDialog.h"

class Analyzer:public QApplication
{

	Q_OBJECT

private:
	bool autoDeath;
	ConnectionModel model;
	MainWindow * window;
	QPointer<IDevList> deviceList;
	QPointer<IDevice> activeDevice;
	QPluginLoader *  snifferPlugin;
	QHash<Packet, Connection* > connections;
	QCache<QHostAddress, QString> dnsCache;
	RManager recognizers;

	Analyzer(const Analyzer & analyzer);
	const Analyzer& operator=(const Analyzer& analyzer);
	

public:
	
	Analyzer(int& argc, char** argv);
	~Analyzer();

signals:
	void analyzed(Connection * con);
	void devsChanged(QStringList ndevs);

public slots:
	void error(QString error);
	void showOptions();
	bool loadSniffer(QString path = "");
	void analyze(IDevice * dev, QByteArray data);
	void deepAnalyze();
	bool selectDevice(int num);
	bool setAutoPurge(bool on);
	void addDnsRecord(QHostAddress address, QString name);
};
#endif

