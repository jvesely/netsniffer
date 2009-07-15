/*!
 * @file
 * @brief Short description.
 *
 * Long description. I would paste some Loren Ipsum rubbish here, but I'm afraid
 * It would stay that way. Not that this comment is by any means ingenious but
 * at least people can understand it.
 */
#pragma once

template class QVector<QString>;
class IDevice;

class IDeviceList: public QObject
{
public:
	virtual uint count() const = 0;
	inline IDevice* operator[] ( uint num )
		{ return device( num ); };
	virtual IDevice* device( uint num ) const = 0;
	virtual const QStringList getNames() const = 0;
	virtual inline ~IDeviceList(){};
};

Q_DECLARE_INTERFACE(IDeviceList,
                     "Network Interface List/1.0") 
