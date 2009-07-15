#pragma once

#include <stdexcept>

#include "NetworkInfo.h"
#include "IRecognizer.h"

class IConnection: public QObject, public QSharedData
{
	Q_OBJECT
public:
	typedef QExplicitlySharedDataPointer<IConnection> Pointer;
	
	enum Status {	Alive, Dead, TimedOut, Closed	};
	enum Direction { Forward, Back };
	
	typedef QPair<Direction, QByteArray> DirectedPacket;
	typedef QPair<uint, uint> PacketCount;
	typedef QPair<uint, uint> Speed;
	typedef QPair<quint64, quint64> DataCount;

	inline IConnection(): mRecognizer( NULL ) {};
	virtual ~IConnection() {};
	virtual const NetworkInfo& networkInfo() const = 0;
	virtual const DirectedPacket nextPacket() throw(std::underflow_error) = 0;
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
