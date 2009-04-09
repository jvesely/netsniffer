#pragma once

class IConnection;
class IRecognizer;
class IDevice;
class IDeviceList;
class IOptionsPage;
class PluginLoader;

class QWidget;

typedef QList<PluginLoader*> PluginList;

class IAnalyzer
{
public:
	virtual ~IAnalyzer(){};
	virtual IDevice * currentDevice() const = 0;
	virtual QAbstractItemModel * model()  = 0;
	virtual const QStringList devices() const = 0;
	virtual const PluginList currentPlugins() const = 0;
	virtual IConnection * connection( QModelIndex ) = 0;

	virtual bool registerDeviceList( IDeviceList* ) = 0;
	virtual void registerOptionsPage( IOptionsPage* ) = 0;

/*
public slots:
	virtual bool selectDevice( int device ) = 0;
	virtual bool setAutoPurge( bool on ) = 0;
	virtual void purge() = 0;
	virtual bool loadPlugin(QString path) = 0;
//	virtual bool addRecognizerPlugin(QString path) = 0;
	virtual void saveSettings() = 0;

signals:
	void recognizerAdded(IRecognizer * newRec);
	void devicesChanged(QStringList newDevices);
	void deviceChanged(IDevice * newDevice);
	void error(QString text);
*/
};
