#include "ConnectionModel.h"
#include "IDNSCache.h"

#define DEBUG_TEXT "[ ConnectionModel ]:"
#include "debug.h"

static const QVariant UNKNOWN = QVariant( QString( "Not recognized." ) );

/*----------------------------------------------------------------------------*/
inline QString formatByteSize( quint64 size )
{
	static const QString suffix[] = { "B", "kB", "MB", "GB", "TB", "PB", "EB" };
	
	double count = size;
	int suf = 0;

	while ((count > 1024) && (suf < 7))
	{
		count = count / 1024;
		++suf;
	}

	return QLocale::system().toString( count, 'f', 1 ) + " " + suffix[suf] + " " + QString::number( size );

}
/*----------------------------------------------------------------------------*/
QVariant ConnectionModel::headerData(int section, Qt::Orientation orientation, int role) const
{

	static const QString names[] = { "Network Info", "Packets", "Speed", "Comment" };
	if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
		return names[section];
	 
	return QVariant();
}
/*----------------------------------------------------------------------------*/
inline const QVariant ConnectionModel::networkData( const NetworkInfo& info, int role ) const
{
	static const QIcon icons[] =
	    { QIcon( ":/net/tcp.png" ), QIcon( ":/net/udp.png" ) };

	switch (role)
	{
		case Qt::DisplayRole:
			return
          QString("From: %1:%3\nTo: %2:%4").
            arg( m_dns->translate( info.sourceIP ) ).
            arg( m_dns->translate( info.destinationIP ) ).
            arg( info.sourcePort ).arg( info.destinationPort );

		case Qt::DecorationRole:
			return info.protocol == TCP ?  icons[0] : icons[1];		

		case Qt::SizeHintRole:
			return 250;

		default:
			return QVariant();

	}
}
/*----------------------------------------------------------------------------*/
inline const QVariant ConnectionModel::packetsData( const IConnection::Pointer connection, int role ) const
{
	Q_ASSERT( connection );
	switch (role)
	{
		case Qt::DisplayRole:
		{
			IConnection::PacketCount count = connection->totalPackets();
			return QString("Fw: %1\nBk: %2").arg( 
				QLocale::system().toString( count.first ),
				QLocale::system().toString( count.second )
			);
		}

		case Qt::ToolTipRole:
		{
			IConnection::DataCount data = connection->countData();
			return QString( "Fw: %1\nBk: %2" ).arg(
				formatByteSize( data.first ), formatByteSize( data.second )
			);
		}
		default:
			return QVariant();
	}
}
/*----------------------------------------------------------------------------*/
inline const QVariant ConnectionModel::speedData( const IConnection::Pointer connection, int role ) const
{
	const IConnection::Speed speed = connection->speed();
	switch (role)
	{
		case Qt::DisplayRole:
			return QString("Fw: %1/s\nBk: %2/s").arg(
				formatByteSize( speed.first ), formatByteSize( speed.second )
			);
		default:
			return QVariant();
	}
}
/*----------------------------------------------------------------------------*/
inline const QVariant ConnectionModel::commentData( const IConnection::Pointer connection, int role ) const
{
	switch (role)
	{
		case Qt::DisplayRole:
			return connection->comment( "Not recognized" );
		case Qt::ToolTipRole:
			return connection->recognizer()
				? connection->recognizer()->name() : "No suitable recognizer was found";
		default:
			return QVariant();
	}
}
/*----------------------------------------------------------------------------*/
QVariant ConnectionModel::data( const QModelIndex& index, int role ) const
{
	Q_ASSERT (index.row() >= 0 && index.row() < m_connections.count());

	const IConnection::Pointer connection = m_connections.at( index.row() );

	const NetworkInfo& info = connection->networkInfo();
	
	switch (role) {
		case Qt::BackgroundRole:
			{
				const IConnection::ConnectionStatus status = connection->status();
				Q_ASSERT( status < 4 );
				static const QVariant colours[] =
					{ QVariant(), QColor("#d4dfe6"), QVariant(), QColor("#d4e6d6") };
				return colours[status];
			}

		default:
			switch (index.column())
			{
				case Column::Network: // adresses or dns
					return networkData( info, role );
				
				case Column::PacketsCount: //packets
					return packetsData( connection, role );
				
				case Column::Speed: //speed
					return speedData( connection, role );
				
				case Column::Comment: //fourth column: comment
					return commentData( connection, role );

				default:
					Q_ASSERT( !"No Such column" );
			}
	}

	return QVariant();
}
/*----------------------------------------------------------------------------*/
bool ConnectionModel::insertConnection( IConnection::Pointer connection )
{
	connect( connection.data(), SIGNAL(statusChanged( IConnection::Pointer )), 
		this, SLOT(updateConnection( IConnection::Pointer )) );
	connect( connection.data(), SIGNAL(finished( IConnection::Pointer )),
		this, SLOT(removeConnection( IConnection::Pointer )) );
	//connect( connection.data(), SIGNAL( packetArrived( IConnection::Pointer )),
		//this, SLOT(updateConnection( IConnection::Pointer )) );

	const int pos = m_connections.count();
	//m_index[connection] = pos;

	beginInsertRows( QModelIndex(), pos, pos);
	m_connections.append( connection );
	endInsertRows();

	return true;
} 
/*----------------------------------------------------------------------------*/
bool ConnectionModel::updateConnection( const IConnection::Pointer connection )
{
	Q_ASSERT (connection);
	
	const int i = //m_index[connection];
		m_connections.indexOf( connection );

	if ( i == -1 )
		return false;

	//netowrk info is treated sepparatelly
	emit dataChanged( index( i, 1 ), index( i, Column::COUNT - 1 ) );
	return true;
}
/*----------------------------------------------------------------------------*/
bool ConnectionModel::removeConnection( IConnection::Pointer corpse )
{

	const int i = m_connections.indexOf( corpse );
	if ( i == -1 )
		return false;

	beginRemoveRows( QModelIndex(), i, i );
	m_connections.removeAt( i );
	endRemoveRows();

	return true;
}
/*----------------------------------------------------------------------------*/
void ConnectionModel::DNSRefresh()
{
	emit dataChanged( createIndex( 0, Column::Network ), 
		createIndex( m_connections.count(), Column::Network ) );
}
