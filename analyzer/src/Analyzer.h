#pragma once

#define NAME "IPAnalyzer"
#define COMPANY "Orome"

#include "ConnectionModel.h"
#include "IRecognizer.h"
#include "IAnalyzer.h"
#include "IDevice.h"
#include "IDevList.h"
#include "Packet.h"
#include "Connection.h"
#include "RManager.h"
#include "MainWindow.h"
#include "SorterPool.h"
#include "SafeHash.h"

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
	//SafeQueue<QByteArray> packets;
	//SafeHash<Packet, QPointer<Connection> > connections;
	QCache<QHostAddress, QString> dnsCache;
	SorterPool  sorters;

	RManager recognizers;

	Analyzer(const Analyzer & analyzer);
	const Analyzer& operator=(const Analyzer& analyzer);

	void loadSettings();
	

public:
	
	Analyzer(int& argc, char** argv);
	~Analyzer();
	inline IDevice * currentDevice() const {return activeDevice;};
	inline QAbstractItemModel * model() { return &model_;};
	inline const QStringList devices() const 
			{ return deviceList?deviceList->getList():QStringList();};
	inline const QStringList engines() const
			{ return QStringList(); };
#warning WARNING: engine list impelment;
	inline const QList<IRecognizer * > currentRecognizers()
			{ return recognizers.currentRecognizers(); };
	inline IConnection * connection(QModelIndex index) 
			{ return model_.connection(index); };

signals:
	void sendAutoPurge(bool on);
	void update();

public slots:
	//void handleError(QString error);
	bool loadSnifferPlugin(QString path);
	inline bool addRecognizerPlugin(QString path)
		{ return recognizers.addRecognizer(path); }; 
	void saveSettings();
	void addConnection(Connection * conn);
	void addPacket(IDevice *dev, QByteArray data);
	bool selectDevice(int num);
	bool setAutoPurge(bool on);
	void purge();
	void addDnsRecord(QHostAddress address, QString name);
};
