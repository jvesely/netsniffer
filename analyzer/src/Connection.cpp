#include "Connection.h"

#define DEBUG_TEXT "[ Connection ]:"
#include "debug.h"

static const int TIMEOUT_INTERVAL = 5;

Connection::Connection( const Packet& packet ):
	mTimeout( TIMEOUT_INTERVAL ), mStatus( Alive ),
	mInfo( packet.networkInfo() ),	mCountForward( 1 ), mCountBack( 0 ),
	mSpeedDown( 0 ), mDataDown( 0 )
{
	mData.enqueue( DirectedPacket( Forward, packet.data() ) );
	mSpeedUp = mDataUp = packet.data().size();
	PRINT_DEBUG ("-------------Creating" << this << "-----------");
}
/*----------------------------------------------------------------------------*/
Connection::~Connection()
{
	QWriteLocker locker( &mGuard ); // wait if something is by any chance inserting packet
	PRINT_DEBUG ("----------------Dying" << this << "--------------");
}
/*----------------------------------------------------------------------------*/
void Connection::update()
{
	QWriteLocker lock( &mGuard );
	mSpeedUp = mSpeedDown = 0;
}
/*----------------------------------------------------------------------------*/
void Connection::close()
{
	mStatus = Closed;
	QTimer::singleShot( mTimeout * 1000, this, SLOT(die()) );
	emit statusChanged( IConnection::Pointer( this ) );
	PRINT_DEBUG ("Closed connection.." << this);
}
/*----------------------------------------------------------------------------*/
void Connection::die()
{
	if (mStatus != Closed)
		return;

	PRINT_DEBUG ("Connection dying" <<  this);
	mStatus = Dead;

	if (mRemoveDead) {
		emit finished( IConnection::Pointer( this ) );
	} else {
		emit statusChanged( IConnection::Pointer( this ) );
	}
}
/*----------------------------------------------------------------------------*/
bool Connection::setAutoPurge( bool on )
{
	bool ret = mRemoveDead;
	{
		QWriteLocker lock( &mGuard );
		mRemoveDead = on;
		PRINT_DEBUG ("Setting Autopurge " << mRemoveDead << "for: " << this);
	}
	if (mStatus == Dead && on)
		emit finished( IConnection::Pointer( this ) );
	return ret;
}
/*----------------------------------------------------------------------------*/
const IConnection::PacketCount Connection::waitingPackets() const
{
	return PacketCount( 0, 0 );
}
/*----------------------------------------------------------------------------*/
bool Connection::addPacket( const Packet& packet )
{
	{
		QWriteLocker lock( &mGuard );
		mStatus = Alive;

		if (myWay( packet ))
		{
			mData.enqueue( DirectedPacket( Forward, packet.data() ) );

			mSpeedUp += packet.data().size();
			mDataUp += packet.data().size();
			PRINT_DEBUG ("ADDED " << packet.data().size() << "BYTES");
			++mCountForward;
		} else

		if (backWay( packet ))
		{
			mData.enqueue( DirectedPacket( Back, packet.data() ) );

			mSpeedDown += packet.data().size();
			mDataDown += packet.data().size();
			PRINT_DEBUG ("ADDED " << packet.data().size() << "BYTES");
			++mCountBack;
		} else
		{
			Q_ASSERT (!"Wrong way packet");
		}

		while (mData.count() > MAX_PACKETS_IN_QUEUE )
		{
			mData.dequeue();
		}

	}
	emit packetArrived( IConnection::Pointer( this ) );
	return true;
}
