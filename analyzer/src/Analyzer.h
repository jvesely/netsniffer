#pragma once

#include "Connection.h"
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

typedef SafeHash<NetworkInfo, Connection::Pointer> ConnectionTable;

class Analyzer:public IAnalyzer, public Singleton<Analyzer>
{
	friend class Singleton<Analyzer>;
public:
	typedef QList<PluginLoader*> PluginList;
	~Analyzer();

	inline IDevice* currentDevice() const { return m_activeDevice; };
	inline const PluginList plugins() const { return m_plugins; };

	const QStringList deviceNames() const
		{ return m_deviceList ? m_deviceList->getNames():QStringList(); };

	bool registerOptionsTab( IOptionsTab* tab );
	bool unregisterOptionsTab( IOptionsTab* tab );
	const OptionsList registeredOptionTabs()
		{ return m_options; };

	bool registerRecognizer( IRecognizer* recognizer );
	void unregisterRecognizer( IRecognizer* recognizer );
	const RecognizerList registeredRecognizers()
		{ return m_recognizers; };

	IDNSCache* dnsCache()
		{ return &m_dnsCache; }

public slots:
	bool addPlugin( const QString& file );
	void removePlugin( QObject* plugin );
	
	void saveSettings();
	bool registerDeviceList( IDeviceList* = NULL );

	void addConnection( Connection::Pointer connection );
	void removeConnection( IConnection::Pointer connection );
	void packetConnection( IConnection::Pointer connection );

	void addPacket( IDevice* dev, QByteArray data );

	bool selectDevice( int num );
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
