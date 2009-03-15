#pragma once

class IDevice;
class IDeviceList: public QObject
{
public:
	virtual uint getCount() const = 0;
	IDevice * operator[] ( uint num )
		{ return device( num ); };
	virtual IDevice * device( uint num ) const = 0;
	virtual const QStringList getNames() const = 0;
	virtual inline ~IDeviceList(){};
};

Q_DECLARE_INTERFACE(IDeviceList,
                     "Network Interface List/1.0") 