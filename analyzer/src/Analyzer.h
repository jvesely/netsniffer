#ifndef _ANALYZER_H_
#define _ANALYZER_H_

#include <QObject>
#include <QByteArray>
#include <QHash>
#include <QCache>
#include <QPluginLoader>
#include <QApplication>
#include "IDevice.h"
#include "IDevList.h"
#include "Packet.h"
#include "Connection.h"
#include "RManager.h"
#include "MainWindow.h"
#include "OptionsDialog.h"

class Analyzer:public QApplication{
	Q_OBJECT
private:
	bool autoDeath;
	MainWindow * window;
	OptionsDialog * options;
	QPointer<IDevList> list;
	QPointer<IDevice> dev;
	QPluginLoader *  snifferPlg;
	QHash<Packet, Connection* > connections;
	QCache<QHostAddress, QString> dns;
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
	void showOptions();
	bool loadSniffer(QString path = "");
	void analyze(IDevice * dev, QByteArray data);
	bool selectNIC(int num);
	void setAutoDeath(bool on);
};
#endif

