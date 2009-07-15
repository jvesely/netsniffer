/*!
 * @file
 * @brief Short description.
 *
 * Long description. I would paste some Loren Ipsum rubbish here, but I'm afraid
 * It would stay that way. Not that this comment is by any means ingenious but
 * at least people can understand it.
 */
#pragma once

#include "IRecognizer.h"
#include "IConnection.h"
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
	typedef QList<IConnection::Pointer> ConnectionList;

	virtual ~IAnalyzer() {};
	virtual IDevice* currentDevice() const = 0;
	virtual const QStringList deviceNames() const = 0;
	virtual IDNSCache* dnsCache() = 0;
	virtual const IDNSCache* dnsCache() const = 0;

	virtual bool registerOptionsTab( IOptionsTab* tab ) = 0;
	virtual bool unregisterOptionsTab( IOptionsTab* tab ) = 0;
	virtual const OptionsList registeredOptionTabs() = 0;

	virtual bool registerRecognizer( IRecognizer* recognizer ) = 0;
	virtual void unregisterRecognizer( IRecognizer* recognizer ) = 0;
	virtual const RecognizerList registeredRecognizers() = 0;

	virtual const ConnectionList connections() = 0;
	virtual void removeConnection( IConnection::Pointer connection ) = 0;

public slots:
	virtual bool addPlugin( const QString& path) = 0;
	virtual void saveSettings() = 0;
	virtual bool registerDeviceList( IDeviceList* = NULL ) = 0;
	virtual bool selectDevice( int device ) = 0;
//	virtual bool setAutoPurge( bool on ) = 0;

signals:
	void newConnection( IConnection::Pointer connection );
	void devicesChanged( const QStringList new_devices);
	void deviceChanged( IDevice* new_device);
	void error( const QString text );
	void newOptionsTab( IOptionsTab* options_page );
};
