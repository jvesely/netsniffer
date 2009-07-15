/*!
 * @file
 * @brief Short description.
 *
 * Long description. I would paste some Loren Ipsum rubbish here, but I'm afraid
 * It would stay that way. Not that this comment is by any means ingenious but
 * at least people can understand it.
 */
#pragma once

#include <stdexcept>

#include "NetworkInfo.h"
#include "IRecognizer.h"

/*!
 * @class IConnection IConnection.h "IConnection.h"
 * @brief Connection interface class.
 *
 * Defines interface for Connection class. Connection class shall represent 
 * captured packets that belongs to the same connection. Class emits signals:
 * packetArrived when new packet has been inserted into it's queue.
 * statusChanged when the connection changes its status i.e. alive->closed
 * finished
 */
class IConnection: public QObject, public QSharedData
{
	Q_OBJECT
public:
	/*!
	 * @typedef Pointer
	 * @brief Pointer part of reference counted pointers infrastructure used by 
	 * IConnection class.
	 */
	typedef QExplicitlySharedDataPointer<IConnection> Pointer;

	/*!
	 * @enum Status
	 * @brief Possible states of the connection.
	 */	
	enum Status {	Alive, Dead, TimedOut, Closed	};

	/*!
	 * @enum ConnectionStatus
	 * @brief Possible states of the connection.
	 */
	enum ConnectionStatus {	Alive, Dead, TimedOut, Closed	};

	/*!
	 * @enum Direction
	 * @brief Possible directions.
	 */
	enum Direction { Forward, Back };

	/*!
	 * @typedef DirectedPacket
	 * @brief Packet data coupled with the direction in which it was captured.
	 */
	typedef QPair<Direction, QByteArray> DirectedPacket;

	/*!
	 * @typedef PacketCount
	 * @brief Number of captured packets in both directions, Forward first, Back second.
	 */
	typedef QPair<uint, uint> PacketCount;

	/*!
	 * @typedef Speed
	 * @brief Byte captured durgin last second, both directions.
	 */
	typedef QPair<uint, uint> Speed;
	
	/*!
	 * @typedef DataCount
	 * @brief Number of captured bytes in both directions, Forward first, Back second.
	 */
	typedef QPair<quint64, quint64> DataCount;

	/*!
	 * @brief Sets currently assigned recognizer to NULL.
	 */
	inline IConnection(): mRecognizer( NULL ) {};

	/*!
	 * @brief Virtual destructor for the sake of inheritance.
	 */
	virtual ~IConnection() {};

	/*!
	 * @brief Gets NetworkInfo that identifies this connection.
	 * @return Reference to NetworkInfo identifying this connection.
	 */
	virtual const NetworkInfo& networkInfo() const = 0;

	/*!
	 * @brief Retrieves next DirectedPacket stored in the queue.
	 * @return Oldest stored packet.
	 * @exception std::runtime_error if there is no packet in the queue.
	 * @note Retrieved packet is remomved from the queue.
	 */
	virtual const DirectedPacket nextPacket() throw(std::underflow_error) = 0;

	/*!
	 * @brief Retrieves copy of the oldest packet stored in the queue.
	 * @return Oldest stored packet.
	 * @exception std::runtime_error if there is no packet in the queue.
	 * @note Packet stays in the queue.
	 */
	virtual const DirectedPacket topPacket() const throw(std::runtime_error) = 0;
	virtual const DirectedPacket topPacket() const throw(std::underflow_error) = 0;

	virtual const DataCount countData() const = 0;
	virtual const PacketCount totalPackets() const = 0;
	virtual const PacketCount waitingPackets() const = 0;
	virtual const Speed speed() const = 0;
	virtual Status status() const = 0;

	inline const QVariant comment( const QVariant& no_comment = "Not Recognized" )
		{ return mRecognizer ? mRecognizer->comment( this ) : no_comment; }

	inline bool showDetails()
		{ return mRecognizer ? mRecognizer->showDetails( this ) : false; }

	inline void setRecognizer( IRecognizer* recognizer )
		{ mRecognizer = recognizer; }
	
	inline IRecognizer* recognizer() const
		{ return mRecognizer; }

public slots:
	virtual void close() = 0;
	virtual void die() = 0;

signals:
	void packetArrived( IConnection::Pointer me );
	void statusChanged( IConnection::Pointer me, IConnection::Status status );

protected:
	QPointer<IRecognizer> mRecognizer;
};
