#pragma once

#include "DNSCache.h"
#include "IAnalyzer.h"
#include "IConnection.h"
#include "IDevice.h"
#include "IDeviceList.h"
#include "IRecognizer.h"
#include "Packet.h"
#include "PluginLoader.h"
#include "Updater.h"
#include "Singleton.h"
#include "struct/SafeHash.h"
#include "gui/PluginCenter.h"

typedef SafeHash<NetworkInfo, IConnection::Pointer> ConnectionTable;

class Analyzer:public IAnalyzer, public Singleton<Analyzer>
{
	friend class Singleton<Analyzer>;
public:
	typedef QList<PluginLoader*> PluginList;
	~Analyzer();

	inline IDevice* currentDevice() const { return mActiveDevice; };
	inline const PluginList plugins() const { return mPlugins; };

	const QStringList deviceNames() const
		{ return mDeviceList ? mDeviceList->getNames():QStringList(); };

	bool registerOptionsTab( IOptionsTab* tab );
	bool unregisterOptionsTab( IOptionsTab* tab );
	const OptionsList registeredOptionTabs()
		{ return mOptions; };

	bool registerRecognizer( IRecognizer* recognizer );
	void unregisterRecognizer( IRecognizer* recognizer );
	const RecognizerList registeredRecognizers()
		{ return mRecognizers; };

	IDNSCache* dnsCache()
		{ return &mDnsCache; }

	const IDNSCache* dnsCache() const
		{ return &mDnsCache; }

	const ConnectionList connections()
		{ return mConnections.values(); }

public slots:
	bool addPlugin( const QString& file );
	void removePlugin( QObject* plugin );
	
	void saveSettings();
	bool registerDeviceList( IDeviceList* list = NULL );

	bool addConnection( IConnection::Pointer connection );
	void removeConnection( IConnection::Pointer connection );
	void packetConnection( IConnection::Pointer connection );

	void addPacket( IDevice* dev, QByteArray data );

	bool selectDevice( int num );

	void loadSettings();

signals:
	void newPlugin( PluginLoader* plugin );

protected:
	Analyzer();

private:
	DNSCache mDnsCache;
	IDeviceList* mDeviceList;
	QPointer<IDevice> mActiveDevice;

	PluginList   mPlugins;
	OptionsList  mOptions;
	RecognizerList mRecognizers;
	PluginCenter mPluginOptions;

	QThreadPool mWorkers;
	ConnectionTable mConnections;
	Updater mUpdater;

	Q_OBJECT;
	Q_DISABLE_COPY (Analyzer);
};

#define ANALYZER Analyzer::instance()
