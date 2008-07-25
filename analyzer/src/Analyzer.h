#pragma once

#include "qtInc.h"
#include "ConnectionModel.h"
#include "IRecognizer.h"
#include "IAnalyzer.h"
#include "IDevice.h"
#include "IDevList.h"
#include "Packet.h"
#include "Connection.h"
#include "RManager.h"
#include "MainWindow.h"

class Analyzer:public IAnalyzer
{

	Q_OBJECT

private:
	bool autoDeath;
	ConnectionModel model_;
	MainWindow * window;
	QPointer<IDevList> deviceList;
	QPointer<IDevice> activeDevice;
	QPluginLoader *  snifferPlugin;
	QHash<Packet, QPointer<Connection> > connections;
	QCache<QHostAddress, QString> dnsCache;
	
	RManager recognizers;

	Analyzer(const Analyzer & analyzer);
	const Analyzer& operator=(const Analyzer& analyzer);
	

public:
	
	Analyzer(int& argc, char** argv);
	~Analyzer();
	inline IDevice * currentDevice() const {return activeDevice;};
	inline QAbstractItemModel * model() { return &model_;};
	inline const QStringList devices() const { return deviceList?deviceList->getList():QStringList();};
	inline const QList<IRecognizer * > currentRecognizers(){return recognizers.currentRecognizers();};

signals:
	void sendAutoPurge(bool on);

public slots:
	//void handleError(QString error);
	bool loadSnifferPlugin(QString path);
	inline bool addRecognizerPlugin(QString path){ return recognizers.addRecognizer(path);}; 
	void analyze(IDevice * dev, QByteArray data);
	void deepAnalyze();
	bool selectDevice(int num);
	bool setAutoPurge(bool on);
	void purge();
	void addDnsRecord(QHostAddress address, QString name);
};
