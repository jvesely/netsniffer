#pragma once

// remove this
#define NAME "IPAnalyzer"
#define COMPANY "Orome"

#include "Connection.h"
#include "ConnectionModel.h"
#include "DNSCache.h"
#include "IAnalyzer.h"
#include "IDevice.h"
#include "IDeviceList.h"
#include "Updater.h"
#include "Packet.h"
#include "PluginLoader.h"
#include "struct/SafeHash.h"

#include "gui/PluginCenter.h"

class MainWindow;
class IRecognizer;
class IOptionsPage;

typedef QList<IOptionsPage*> OptionsList;

typedef SafeHash<NetworkInfo, QSharedPointer<Connection> > ConnectionTable;
typedef QSet<Connection*> ConnectionSet;

typedef QList<IRecognizer*> RecognizerList;
typedef SafeHash<Connection*, QPointer<IRecognizer> > RecognizerTable;

class Analyzer:public QApplication, public IAnalyzer
{
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

	inline QAbstractItemModel* model() { return &m_model; };
	inline IDevice* currentDevice() const { return m_activeDevice; };
	inline const PluginList currentPlugins() const { return m_plugins; };

	inline const QStringList deviceNames() const 
		{ return m_deviceList ? m_deviceList->getNames():QStringList(); };

	inline const OptionsList& registeredOptions()
		{ return m_options; };
	
	bool registerOptionsPage( IOptionsPage* new_options );
	
	bool registerRecognizer( IRecognizer* recognizer );

	inline IConnection* connection( QModelIndex index ) 
		{ return m_model.connection( index ); };
	
	IDNSCache* dnsCache()
		{ return &m_dnsCache; }

public slots:
	bool addPlugin( const QString& file );
	void removePlugin( QObject* plugin );
	
	void saveSettings();
	bool registerDeviceList( IDeviceList* = NULL );

	void addConnection( Connection* connection );
	void removeConnection( Connection* connection );
	void packetConnection( Connection* connection );

	void addPacket( IDevice* dev, QByteArray data );

	bool selectDevice( const int num );
	bool setAutoPurge( bool on );

private:
	bool m_autoDeath;
	ConnectionModel m_model;
	DNSCache m_dnsCache;
	MainWindow* m_window;
	IDeviceList* m_deviceList;
	QPointer<IDevice> m_activeDevice;

	PluginList   m_plugins;
	OptionsList  m_options;
	PluginCenter m_pluginOptions;

	QThreadPool m_workers;
	ConnectionTable m_connections;
	ConnectionSet m_waitingConnection;

	RecognizerList m_analyzers;
	RecognizerTable m_lastUsedAnalyzers;

	Updater updater;

	Q_OBJECT;
	Q_DISABLE_COPY (Analyzer);

	void loadSettings();	
};

#define ANALYZER static_cast<Analyzer*>(qApp)
