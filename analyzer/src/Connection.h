#pragma once

#include "Packet.h"
#include "IConnection.h"

typedef QQueue<IConnection::DirectedPacket> DirectedPacketQueue;

class Connection:public IConnection
{
public slots:
	void close();
	void die();
	virtual void update();

public:
	typedef QExplicitlySharedDataPointer<Connection> Pointer;

	Connection( const Packet& packet );
	~Connection();

	inline Connection& operator << ( const Packet& packet )
		{ return addPacket( packet ),*this; }

	inline const NetworkInfo& networkInfo() const throw()
		{ return mInfo; };

	inline Status status() const throw()
		{ return mStatus; };

	inline const DirectedPacket nextPacket() throw (std::underflow_error)
		{
			QReadLocker lock( &mGuard );
			if (mData.isEmpty()) throw std::underflow_error( "Empty queue" );
			return mData.dequeue();
		}
	
	inline const DirectedPacket topPacket() const throw (std::underflow_error)
		{
			QReadLocker lock( &mGuard );
			if (mData.isEmpty()) throw std::underflow_error( "Empty queue" );
			return mData.head();
		}

	inline const DataCount countData() const
		{ return DataCount( mDataUp, mDataDown ); };

	const PacketCount waitingPackets() const;
	
	inline const PacketCount totalPackets() const
		{ return PacketCount( mCountForward, mCountBack ); };

	inline const Speed speed() const
		{ return Speed( mSpeedUp, mSpeedDown ); };

	virtual bool addPacket( const Packet& packet );

	inline bool myPacket( const Packet& packet )
		{ return packet.networkInfo() == mInfo; }

	inline bool backWay( const Packet& packet )
		{ return ::backWay( mInfo, packet.networkInfo() ); }

	inline bool myWay( const Packet& packet )
		{ return ::sameWay( mInfo, packet.networkInfo() ); }

	static const int MAX_PACKETS_IN_QUEUE = 150;

protected:
	uint mTimeout;
	Status mStatus;

private:
	const NetworkInfo mInfo;
	
	DirectedPacketQueue mData;
	
	uint mCountForward;
	uint mCountBack;
	int mSpeedUp;
	int	mSpeedDown;
	quint64 mDataUp;
	quint64 mDataDown;

	mutable QReadWriteLock mGuard;


	Q_DISABLE_COPY (Connection);
	Q_OBJECT;
};
