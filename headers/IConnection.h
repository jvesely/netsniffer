/*!
 * @file
 * @brief Connection class interface.
 *
 * Provides access to internal queue of packets of this connection, connection
 * status a well as access to the assigned recognizer.
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
	 * @exception std::underflow_error if there is no packet in the queue.
	 * @note Retrieved packet is remomved from the queue.
	 */
	virtual const DirectedPacket nextPacket() throw(std::underflow_error) = 0;

	/*!
	 * @brief Retrieves copy of the oldest packet stored in the queue.
	 * @return Oldest stored packet.
	 * @exception std::underflow__error if there is no packet in the queue.
	 * @note Packet stays in the queue.
	 */
	virtual const DirectedPacket topPacket() const throw(std::underflow_error) = 0;

	/*!
	 * @brief Retrieves total byte-count of the data, in both directions.
	 * @return Byte-count in both directions.
	 */
	virtual const DataCount countData() const = 0;

	/*!
	 * @brief Retrieves total packet-count of the data, in both directions.
	 * @return Pair of packet-counts in forward.
	 */
	virtual const PacketCount totalPackets() const = 0;

	/*!
	 * @brief Retrieves count of packet waiting in the queue, in both directions.
	 * @return Packet-count of packet stored in the internall queue in both
	 * forward and back direction.
	 */
	virtual const PacketCount waitingPackets() const = 0;
	
	/*!
	 * @brief Retrieves current speed.
	 * @return Speed in both directions.
	 * @note Number of bytes captured in the last second.
	 */
	virtual const Speed speed() const = 0;

	/*!
	 * @brief Retrieves current status.
	 * @return Current status.
	 * @note See enum Status.
	 */
	virtual Status status() const = 0;

	/*!
	 * @brief Retrieves comment on this connection,
	 * that is providied by assigned recognizer.
	 * @param no_comment Alternate comment to display if there is no 
	 * assigned recognizer.
	 * @return Comment produced by the assigned recognizer,
	 * or no_comment parameter if there is no assigned recognizer.
	 */
	inline const QVariant comment( const QVariant& no_comment = "Not Recognized" )
		{ return mRecognizer ? mRecognizer->comment( this ) : no_comment; }

	/*!
	 * @brief Displays detials on this connection.
	 * @return Value of IRecognizer::showDetails, if there is recognizer assigned,
	 * false if there is no recognizer assigned to this connection.
	 */
	inline bool showDetails()
		{ return mRecognizer ? mRecognizer->showDetails( this ) : false; }

	/*!
	 * @brief Assigns new recognizer for this connection.
	 * @param recognizer Pointer to the new recognizer,
	 * to be used by this connection.
	 */
	inline void setRecognizer( IRecognizer* recognizer )
		{ mRecognizer = recognizer; }

	/*!
	 * @brief Retrieves pointer to the recognizer currently
	 * used by this connection.
	 * @return Pointer to the currently used recognizer instance,
	 * NULL if there is no such recognizer.
	 */
	inline IRecognizer* recognizer() const
		{ return mRecognizer; }

public slots:
	/*!
	 * @brief Closes connection.
	 * Marks connection as closed and emits statusChanged signal.
	 */
	virtual void close() = 0;
	
	/*!
	 * @brief Kills connection.
	 * Marks connection as dead and emits statusChanged signal.
	 * @note Requires connection to be marked as closed
	 * prior to calling this function.
	 */
	virtual void die() = 0;

signals:
	/*!
	 * @brief Signals arrival of the new packet for this connection.
	 * @param me Counted pointer to self.
	 */
	void packetArrived( IConnection::Pointer me );

	/*!
	 * @brief Signals change of status of the current connection.
	 * @param me Counted pointer to self.
	 * @param status new Status of this connection.
	 */
	void statusChanged( IConnection::Pointer me, IConnection::Status status );

protected:
	QPointer<IRecognizer> mRecognizer;
};
