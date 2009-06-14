#pragma once

class IConnection;
class IRecognizer;
class IDevice;
class IDeviceList;
class IOptionsPage;
class IDNSCache;

class PluginLoader;

typedef QList<PluginLoader*> PluginList;

class IAnalyzer: public QObject
{
	Q_OBJECT
public:
	typedef QList<IOptionsPage*> OptionsList;

	virtual ~IAnalyzer() {};
	virtual IDevice * currentDevice() const = 0;
	virtual QAbstractItemModel * model()  = 0;
	virtual const QStringList deviceNames() const = 0;
	virtual const PluginList currentPlugins() const = 0;
	virtual IDNSCache* dnsCache() = 0;

	virtual bool closeConnection( QModelIndex index ) = 0;
	virtual bool detailConnection( const QModelIndex index ) = 0;

	virtual bool registerOptionsPage( IOptionsPage* options_page ) = 0;
	virtual const OptionsList& registeredOptionPages() = 0;

	virtual bool registerRecognizer( IRecognizer* recognizer ) = 0;
	virtual void unregisterRecognizer( IRecognizer* recognizer ) = 0;

public slots:
	virtual bool addPlugin( const QString& path) = 0;
	virtual void saveSettings() = 0;
	virtual bool registerDeviceList( IDeviceList* = NULL ) = 0;
	virtual bool selectDevice( int device ) = 0;
	virtual bool setAutoPurge( bool on ) = 0;

signals:
	void devicesChanged( const QStringList new_devices);
	void deviceChanged( IDevice* new_device);
	void error( const QString text );
	void newOptionsPage( IOptionsPage* options_page );
};
