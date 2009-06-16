#pragma once

#include "IRecognizer.h"
#include "IDevice.h"
#include "IDeviceList.h"
#include "IOptionsTab.h"

class IDNSCache;

class IAnalyzer: public QObject
{
	Q_OBJECT
public:
	typedef QList<IOptionsTab*> OptionsList;
	typedef QList<IRecognizer*> RecognizerList;

	virtual ~IAnalyzer() {};
	virtual IDevice* currentDevice() const = 0;
	virtual QAbstractItemModel* model()  = 0;
	virtual const QStringList deviceNames() const = 0;
	virtual IDNSCache* dnsCache() = 0;

	virtual bool connectionClose( const QModelIndex index ) = 0;
	virtual bool connectionDetail( const QModelIndex index ) = 0;
//	virtual IRecognizer* connectionRecognizer( const QModelIndex index ) = 0;

	virtual bool registerOptionsTab( IOptionsTab* tab ) = 0;
	virtual bool unregisterOptionsTab( IOptionsTab* tab ) = 0;
	virtual const OptionsList registeredOptionTabs() = 0;

	virtual bool registerRecognizer( IRecognizer* recognizer ) = 0;
	virtual void unregisterRecognizer( IRecognizer* recognizer ) = 0;
	virtual const RecognizerList registeredRecognizers() = 0;

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
	void newOptionsTab( IOptionsTab* options_page );
};
