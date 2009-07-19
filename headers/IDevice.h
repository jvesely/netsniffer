/*!
 * @file
 * @brief IDevice class interface.
 *
 * Provides interface for Device class. Device shall be able to
 * identify itself by name and possibly short description. It shall
 * provide control for the manipulation of the cpature session.
 */
#pragma once

/*!
 * @class IDevice IDevice.h "IDevice.h"
 * @brief Device interface class.
 *
 * Provides interface for Device class representing network device.
 * Shall provide access to name and short description, as well as 
 * some statistics and capture control interface.
 */
class IDevice: public QObject
{
	Q_OBJECT;

public:
	/*!
	 * @struct Stats
	 * @brief Packet Statistics.
	 */
	struct Stats
	{
		uint received;   /*< Recieved packets */
		uint dropped;    /*< Dropped by capture filter */
		uint ifdropped;  /*< Dropped by interface (not supported on Windows */
	};

	/*!
	 * @brief Virtual destructor for the sake of inheritance.
	 */
	virtual inline ~IDevice() {};

	/*!
	 * @brief Gets name of the device.
	 */
	virtual const QString getName() const = 0;

	/*!
	 * @brief Gets description of the device.
	 */
	virtual const QString getDescription() const = 0;

	/*!
	 * @brief Gets device stats.
	 */
	virtual const Stats getStats() const = 0;

public slots:
	/*!
	 * @brief Starts capture.
	 * @return True if capture was sucessfully started, false otherwise.
	 */
	virtual bool captureStart() = 0;
	
	/*!
	 * @brief Stops capture.
	 * @return True if capture was sucessfully stopped, false otherwise.
	 */
	virtual bool captureStop()  = 0;

signals:
	/*!
	 * @brief Signals new captured packet.
	 * @param me Pointer to self.
	 * @param data Data of the newly captured packet.
	 */
	void packetArrived( IDevice* me, QByteArray data );

	/*!
	 * @brief Signals start of the capture session.
	 * @param me Pointer to self.
	 */
	void captureStarted( IDevice* me );

	/*!
	 * @brief Signals end of the capture session.
	 * @param me Pointer to self.
	 */
	void captureStopped( IDevice* me );
};
