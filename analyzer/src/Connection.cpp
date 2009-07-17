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
	mSpeedUp = mSpeedDown = 0;
}
/*----------------------------------------------------------------------------*/
void Connection::close()
{
	QTimer::singleShot( mTimeout * 1000, this, SLOT(die()) );
	emit statusChanged( IConnection::Pointer( this ), mStatus = Closed );
	PRINT_DEBUG ("Closed connection.." << this);
}
/*----------------------------------------------------------------------------*/
void Connection::die()
{
	if (mStatus == Closed)
	{
		PRINT_DEBUG ("Connection dying" << this);
		emit statusChanged( IConnection::Pointer( this ), mStatus = Dead );
	}
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
			/* do not polute queue with empty packets */
			if (!packet.data().isEmpty())
				mData.enqueue( DirectedPacket( Forward, packet.data() ) );

			mSpeedUp += packet.data().size();
			mDataUp += packet.data().size();
			++mCountForward;
		} else

		if (backWay( packet ))
		{
			/* do not polute queue with empty packets */
			if (!packet.data().isEmpty())
				mData.enqueue( DirectedPacket( Back, packet.data() ) );

			mSpeedDown += packet.data().size();
			mDataDown += packet.data().size();
			++mCountBack;
		} else
		{
			Q_ASSERT (!"Wrong way packet");
		}
		PRINT_DEBUG ("ADDED " << packet.data().size() << "BYTES");

		while (mData.count() > MAX_PACKETS_IN_QUEUE )
		{
			try {	mData.dequeue(); }
			catch (...) { break; }
		}

	}
	emit packetArrived( IConnection::Pointer( this ) );
	return true;
}
