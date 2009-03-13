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
#include "PluginCenter.h"

class MainWindow;
class IRecognizer;
class IOptionsPage;

typedef QCache<QHostAddress, QString> DNSCache;
typedef QList<IOptionsPage*> OptionsList;

class Analyzer:public QApplication, public IAnalyzer
{

	Q_OBJECT

signals:
	void sendAutoPurge( bool on );
	void error( QString );
	void deviceChanged( IDevice* new_device );
	void devicesChanged( QStringList new_devices );
	void newPlugin( QPluginLoader* );
	void newOptionsPage( IOptionsPage* );

public:
	
	Analyzer( int& argc, char** argv );
	~Analyzer();

	inline IDevice * currentDevice() const { return activeDevice; };

	inline QAbstractItemModel * model() { return &model_;};

	inline const QStringList devices() const 
		{ return deviceList?deviceList->getNames():QStringList();};

	inline const DNSCache* dns()
		{ return m_dnsCache; }
	
	void registerDNSCache( DNSCache* newCache )
		{ m_dnsCache = newCache; }

	inline const OptionsList& registeredOptions()
		{ return m_options; };
	
	void registerOptionsPage( IOptionsPage* new_options );

	inline const PluginList currentPlugins()
		{ return m_plugins; };

	inline IConnection * connection( QModelIndex index ) 
		{ return model_.connection( index ); };
	
	bool registerDeviceList( IDeviceList * )
		{ return false; };


public slots:
	bool addPlugin( QString file );
	void removePlugin( QObject* plugin );
	
	void saveSettings();


	void addConnection( Connection * conn );
	void addPacket( IDevice *dev, QByteArray data );
	bool selectDevice( int num );
	bool setAutoPurge( bool on );
	void purge();
	void addDnsRecord(QHostAddress address, QString name);

private:
	bool autoDeath;
	ConnectionModel model_;
	MainWindow * window;
	QPointer<IDeviceList> deviceList;
	QPointer<IDevice> activeDevice;
	DNSCache* m_dnsCache;

	PluginList   m_plugins;
	OptionsList  m_options;
	PluginCenter m_pluginOptions;

	SorterPool sorters;
	Updater updater;
	RManager recognizers;
	int updateTimer;

	Q_DISABLE_COPY (Analyzer);

	void loadSettings();	
};
#define ANALYZER static_cast<Analyzer*>(qApp)
