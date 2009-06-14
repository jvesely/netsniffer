#pragma once

#include "Connection.h"
#include "ConnectionModel.h"
#include "DNSCache.h"
#include "IAnalyzer.h"
#include "IDevice.h"
#include "IDeviceList.h"
#include "IRecognizer.h"
#include "Packet.h"
#include "PluginLoader.h"
#include "Updater.h"
#include "Singleton.h"
#include "struct/SafeHash.h"
#include "gui/PluginCenter.h"

//class MainWindow;
class IOptionsPage;

typedef QList<IOptionsPage*> OptionsList;

typedef SafeHash<NetworkInfo, ConnectionPtr > ConnectionTable;

typedef QList<IRecognizer*> RecognizerList;
typedef SafeHash<Connection*, IRecognizer* > RecognizerTable;

class Analyzer:public IAnalyzer, public Singleton<Analyzer>
{
	friend class Singleton<Analyzer>;
public:
	~Analyzer();

	inline QAbstractItemModel* model() { return &m_model; };
	inline IDevice* currentDevice() const { return m_activeDevice; };
	inline const PluginList currentPlugins() const { return m_plugins; };

	const QStringList deviceNames() const
		{ return m_deviceList ? m_deviceList->getNames():QStringList(); };

	const OptionsList& registeredOptionPages()
		{ return m_options; };
	
	bool registerOptionsPage( IOptionsPage* new_options );
	
	bool registerRecognizer( IRecognizer* recognizer );
	void unregisterRecognizer( IRecognizer* recognzier );

	bool closeConnection( const QModelIndex index )
		{ return m_model.connection( index ) 
			? m_model.connection( index )->close(), true : false; };

	bool detailConnection( const QModelIndex index )
		{ return m_model.connection( index )
			? m_model.connection( index )->showDetails(): false; };
	
	IDNSCache* dnsCache()
		{ return &m_dnsCache; }

public slots:
	bool addPlugin( const QString& file );
	void removePlugin( QObject* plugin );
	
	void saveSettings();
	bool registerDeviceList( IDeviceList* = NULL );

	void addConnection( ConnectionPtr connection );
	void removeConnection( ConnectionPtr connection );
	void packetConnection( ConnectionPtr connection );

	void addPacket( IDevice* dev, QByteArray data );

	bool selectDevice( const int num );
	bool setAutoPurge( bool on );

	void loadSettings();

signals:
	void sendAutoPurge( bool on );
	void newPlugin( PluginLoader* plugin );

protected:
	Analyzer();

private:
	bool m_autoDeath;
	DNSCache m_dnsCache;
	ConnectionModel m_model;
	IDeviceList* m_deviceList;
	QPointer<IDevice> m_activeDevice;

	PluginList   m_plugins;
	OptionsList  m_options;
	RecognizerList m_recognizers;
	PluginCenter m_pluginOptions;

	QThreadPool m_workers;
	ConnectionTable m_connections;
	Updater updater;

	Q_OBJECT;
	Q_DISABLE_COPY (Analyzer);
};

#define ANALYZER Analyzer::instance()
