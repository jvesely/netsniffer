#pragma once

#define NAME "IPAnalyzer"
#define COMPANY "Orome"

#include "ConnectionModel.h"
#include "DNSCache.h"
#include "IAnalyzer.h"
#include "IDevice.h"
#include "IDeviceList.h"
#include "RManager.h"
#include "SorterPool.h"
#include "Updater.h"
#include "PluginLoader.h"

#include "gui/PluginCenter.h"

class MainWindow;
class IRecognizer;
class IOptionsPage;

typedef QList<IOptionsPage*> OptionsList;

class Analyzer:public QApplication, public IAnalyzer
{

	Q_OBJECT

signals:
	void sendAutoPurge( bool on );
	void error( QString );
	void deviceChanged( IDevice* new_device );
	void devicesChanged( QStringList new_devices );
	void newPlugin( PluginLoader* plugin );
	void newOptionsPage( IOptionsPage* options_page );

public:
	
	Analyzer( int& argc, char** argv );
	~Analyzer();

	inline IDevice * currentDevice() const { return m_activeDevice; };
	inline const PluginList currentPlugins() const { return m_plugins; };
	inline QAbstractItemModel * model() { return &m_model; };

	inline const QStringList deviceNames() const 
		{ return m_deviceList ? m_deviceList->getNames():QStringList(); };

	inline const OptionsList& registeredOptions()
		{ return m_options; };
	
	bool registerOptionsPage( IOptionsPage* new_options );

	bool registerRecognizer( IRecognizer* recognizer )
		{ return false; };

	inline IConnection* connection( QModelIndex index ) 
		{ return m_model.connection( index ); };
	
	IDNSCache* dnsCache()
		{ return &m_dnsCache; }

public slots:
	bool registerDeviceList( IDeviceList* = NULL );
	bool addPlugin( const QString file );
	void removePlugin( QObject* plugin );
	
	void saveSettings();

	void addConnection( Connection* conn );
	void addPacket( IDevice* dev, QByteArray data );
	bool selectDevice( const int num );
	bool setAutoPurge( bool on );
	void purge();

private:
	bool m_autoDeath;
	ConnectionModel m_model;
	DNSCache m_dnsCache;
	MainWindow* m_window;
	QPointer<IDeviceList> m_deviceList;
	QPointer<IDevice> m_activeDevice;

	PluginList   m_plugins;
	OptionsList  m_options;
	PluginCenter m_pluginOptions;

	SorterPool sorters;
	Updater updater;
	RManager recognizers;

	Q_DISABLE_COPY (Analyzer);

	void loadSettings();	
};

#define ANALYZER static_cast<Analyzer*>(qApp)
