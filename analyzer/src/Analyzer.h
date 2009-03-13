#pragma once

#define NAME "IPAnalyzer"
#define COMPANY "Orome"

#include "ConnectionModel.h"
#include "IAnalyzer.h"
#include "IDevice.h"
#include "IDeviceList.h"
#include "Packet.h"
#include "Connection.h"
#include "RManager.h"
#include "SorterPool.h"
#include "Updater.h"

class MainWindow;
class IRecognizer;

typedef QCache<QHostAddress, QString> DNSCache;

class Analyzer:public QApplication, public IAnalyzer
{

	Q_OBJECT

public:
	
	Analyzer( int& argc, char** argv );
	~Analyzer();

	inline IDevice * currentDevice() const { return activeDevice; };

	inline QAbstractItemModel * model() { return &model_;};

	inline const QStringList devices() const 
		{ return deviceList?deviceList->getNames():QStringList();};

	inline const QStringList engines() const
		{ return QStringList(); };
#warning WARNING: engine list implement;

	inline const QList<QPluginLoader *> currentPlugins()
		{ return plugins; };

	inline IConnection * connection( QModelIndex index ) 
		{ return model_.connection( index ); };
	
	bool registerDeviceList( IDeviceList *)
		{ return false; };

signals:
	void sendAutoPurge( bool on );
	void error( QString );
	void deviceChanged( IDevice* new_device );
	void devicesChanged( QStringList new_devices );
	void newPlugin( QPluginLoader* );

public slots:
//	bool loadSnifferPlugin(QString path);
	bool addPlugin( QString file );
	void removePlugin( QObject* plugin );
//	inline bool addRecognizerPlugin(QString path)
//		{ return recognizers.addRecognizer(path); }; 
	void saveSettings();
	void addConnection(Connection * conn);
	void addPacket(IDevice *dev, QByteArray data);
	bool selectDevice(int num);
	bool setAutoPurge(bool on);
	void purge();
	void addDnsRecord(QHostAddress address, QString name);

private:
	bool autoDeath;
	ConnectionModel model_;
	MainWindow * window;
	QPointer<IDeviceList> deviceList;
	QPointer<IDevice> activeDevice;
	
	QList<QPluginLoader *>  plugins;
	DNSCache dnsCache;
	
	SorterPool sorters;
	Updater updater;
	RManager recognizers;
	int updateTimer;



	Analyzer(const Analyzer & analyzer);
	const Analyzer& operator=(const Analyzer& analyzer);

	void loadSettings();	
};
#define ANALYZER static_cast<Analyzer*>(qApp)
