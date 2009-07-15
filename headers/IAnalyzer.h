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

/*!
 * @class IAnalyzer IAnalyzer.h "IAnalyzer.h"
 * @brief Analyzer interface class.
 *
 * Provides interface for central Analyzer object. Shall provide access
 * to registered plugins, options tabs, and the list of monitored connections.
 */
class IAnalyzer: public QObject
{
	Q_OBJECT
public:
	/*!
	 * @typedef OptionsList
	 * @brief List of pointers to object implementing IOptionsTab interface.
	 *
	 * See registerOptionsTab, unregisterOptionsTab, and registeredOptionTabs.
	 */
	typedef QList<IOptionsTab*> OptionsList;
	
	/*!
	 * @typedef RecognizerList
	 * @brief List of pointers to object implementing IRecognizer interface.
	 *
	 * See registerRecognizer. unregisterRecognizer, and registeredRecognizers.
	 */
	typedef QList<IRecognizer*> RecognizerList;
	
	/*!
	 * @typedef ConnectionList
	 * @brief List of IConnection::Pointers.
	 *
	 * See class IConnection.
	 */
	typedef QList<IConnection::Pointer> ConnectionList;

	/*!
	 * @brief Virtual destructor for the sake of inheritance.
	 */
	virtual ~IAnalyzer() {};

	/*!
	 * @brief Gets currently selected device.
	 * @return Pointer to the currently selected device, NULL if there is no such device.
	 */
	virtual IDevice* currentDevice() const = 0;

	/*!
	 * @brief Gets names of currently available devices.
	 * @return QStringList of names, empty list if no devices are available.
	 */
	virtual const QStringList deviceNames() const = 0;

	/*!
	 * @brief Gets DNSCache that implements IDNSCache interface.
	 * @return Should always return valid pointer to IDNSCache class.
	 */
	virtual IDNSCache* dnsCache() = 0;
	
	/*!
	 * @brief Overloaded function provided for const access.
	 * @return Should always return valid pointer to IDNSCache class.
	 */
	virtual const IDNSCache* dnsCache() const = 0;

	/*!
	 * @brief Adds tab to the list of registered options.
	 * @param tab Pointer to the IOptionsTab object to be added.
	 * @return true if tab was successfully added, false otherwise.
	 * @note You should not delete inserted object,
	 * unless you call unregisterOptionsTab first.
	 * @note Signal newOptionsTab is emitted on success.
	 */
	virtual bool registerOptionsTab( IOptionsTab* tab ) = 0;

	/*!
	 * @brief Removes tab from the list of registered options.
	 * @param tab Pointer to the IOptionsTab object to be removed.
	 * @return true if tab was present and successfully removed, false otherwise.
	 */
	virtual bool unregisterOptionsTab( IOptionsTab* tab ) = 0;

	/*!
	 * @brief Gets list of objects registered by registerOptionsTab.
	 * @return List of registered IOptionsTab objects.
	 */
	virtual const OptionsList registeredOptionTabs() = 0;

	/*!
	 * @brief Adds recognizer to the list of registered recognizers.
	 * @param recognizer Pointer to the IRecognizer object to be added.
	 * @return true if tab was successfully added, false otherwise.
	 * @note You should not delete inserted object,
	 * unless you call unregisterRecognizer first.
	 */
	virtual bool registerRecognizer( IRecognizer* recognizer ) = 0;

	/*!
	 * @brief Removes recognizer from the list of registered recognizers.
	 * @param recognizer Pointer to the IRecognizer object to be removed.
	 * @return true if rrecognizer was present and successfully removed, false otherwise.
	 */
	virtual void unregisterRecognizer( IRecognizer* recognizer ) = 0;

	/*!
	 * @brief Gets list of objects registered by registerRecognizer.
	 * @return List of registered IRecognizer objects.
	 */
	virtual const RecognizerList registeredRecognizers() = 0;

	/*!
	 * @brief Gets list of monitored connections.
	 * @return List IConnection::Pointer objects, list might be empty.
	 */
	virtual const ConnectionList connections() = 0;
	
	/*!
	 * @brief Removes connection from the list of monitored connections.
	 * @param connection Connection to be removed and longer tracked.
	 */
	virtual void removeConnection( IConnection::Pointer connection ) = 0;

public slots:

	/*!
	 * @brief Request Analyzer to add plugin.
	 * @param path Path to the file that contains the plugin,
	 * 	might be both absolute and relative.
	 * @return true if plugin was succesfully added and loaded, false otherwise. 
	 */
	virtual bool addPlugin( const QString& path) = 0;
	
	/*!
	 * @brief Saves all settings, that IAnalyzer needs to remeber between starts.
	 */
	virtual void saveSettings() = 0;

	/*!
	 * @brief Replaces class that provides IDevices.
	 * @param list to be used as source of IDevices.
	 * @return bool if device list was successfully replaced, false otherwise.
	 * @note Signal devicesChanged is emmited if device list was successfully registered.
	 */
	virtual bool registerDeviceList( IDeviceList* list = NULL ) = 0;

	/*!
	 * @brief 
	 * @param device index from the deviceNames() list.
	 * @return bool if device list was successfully selected, false otherwise.
	 * @note Signal deviceChanged is emmited on successfull change.
	 */
	virtual bool selectDevice( int device ) = 0;

signals:
	void newConnection( IConnection::Pointer connection );
	void devicesChanged( const QStringList new_devices);
	void deviceChanged( IDevice* new_device);
	void error( const QString text );
	void newOptionsTab( IOptionsTab* options_page );
};
