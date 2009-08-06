/*!
 * @file
 * @brief Device List interface class.
 *
 * Long description. I would paste some Loren Ipsum rubbish here, but I'm afraid
 * It would stay that way. Not that this comment is by any means ingenious but
 * at least people can understand it.
 */
#pragma once

template class QVector<QString>;
class IDevice;

/*!
 * @class IDeviceList IDeviceList.h "IDeviceList.h"
 * @brief DeviceList interface class.
 *
 * Provides interface for the List of available devices. Provides count,
 * retrieving namelist, as well as individual devices.
 */
class IDeviceList: public QObject
{
public:
	/*!
	 * @brief Number of devices available.
	 * @return Number of devices managed by this list.
	 */
	virtual uint count() const = 0;

	/*!
	 * @brief Retrieves particaular Device.
	 * @param num Number of the device.
	 * @return Pointer to the dvice that prepresents IDevice interface,
	 * NULL on failure.
	 */
	virtual IDevice* device( uint num ) const = 0;

	/*!
	 * @brief Overloaded function provided for convenience.
	 * @param num nuber of dvice to be retrieved.
	 * @return Pointer to the requested device.
	 * @note See IDeviceList::device().
	 */
	inline IDevice* operator[] ( uint num )
		{ return device( num ); };

	/*!
	 * @brief Retrieves list of the names of the avialable devices.
	 * @return QStringList filled with names of the devices.
	 */
	virtual const QStringList getNames() const = 0;

	/*!
	 * @brief Virtual destructor for the sake of inheritance.
	 */
	virtual inline ~IDeviceList(){};
};

Q_DECLARE_INTERFACE(IDeviceList,
                     "Network Interface List/1.0") 
