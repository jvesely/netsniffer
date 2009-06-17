#include "ConnectionModel.h"
#include "IDNSCache.h"

#define DEBUG_TEXT "[ Connection Model ]:"
#include "debug.h"

static const QVariant UNKNOWN = QVariant( QString( "Not recognized." ) );

/*----------------------------------------------------------------------------*/
QVariant ConnectionModel::headerData(int section, Qt::Orientation orientation, int role) const
{

	static const QString names[] = { "Network Info", "Packets", "Speed (B/s)", "Comment" };
	if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
		return names[section];
	 
	return QVariant();
}
/*----------------------------------------------------------------------------*/
inline const QVariant ConnectionModel::networkData( const NetworkInfo& info, int role ) const
{
	static const QIcon icons[] =
	    { QIcon( ":/net/TCP32.png" ), QIcon( ":/net/UDP32.png" ) };

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

	}
}
/*----------------------------------------------------------------------------*/
QVariant ConnectionModel::data( const QModelIndex& index, int role ) const
{
	Q_ASSERT (index.row() >= 0 && index.row() < m_connections.count());

	const IConnection::Pointer connection = m_connections.at( index.row() );
	static const QIcon icons[] =
		{ QIcon( ":/net/TCP32.png" ), QIcon( ":/net/UDP32.png" ) };

	const NetworkInfo& info = connection->networkInfo();
	
	switch (role) {
		case Qt::DisplayRole:
			switch (index.column())
			{
				case ConnectionColumn: // adresses or dns
					return networkData( info, role );
				
				case PacketsCountColumn: //packets
				{
					const IConnection::PacketCount count = connection->totalPackets();
					return QString("Fw: %1\nBk: %2").arg( count.first ).arg( count.second );
				}
				case SpeedColumn: //speed
				{
					const IConnection::Speed speed = connection->speed();
					return QString("Fw: %1\nBk: %2").arg( speed.first ).arg( speed.second );
				}
				case CommentColumn: //fourth column: comment
					return  connection->comment( "Not recognized" );
				default:
					Q_ASSERT( !"No Such column" );
			}

		 case Qt::DecorationRole:
		 	if (index.column() == ConnectionColumn)
				return info.protocol == TCP ?  icons[0] : icons[1];
			break;		

		case  Qt::SizeHintRole:
			if (index.column() == ConnectionColumn)
				return 250;
			break;

		case Qt::BackgroundRole:
			{
				const IConnection::ConnectionStatus status = connection->status();
				Q_ASSERT( status < 4 );
				static const QVariant colours[] =
					{ QVariant(), QColor("#d4dfe6"), QVariant(), QColor("#d4e6d6") };
				return colours[status];
			}
			break;

		case Qt::ToolTipRole:
			switch (index.column())
			{
				case ConnectionColumn: // adresses or dns
				case PacketsCountColumn:
				{
					static const QString suffix[] = { "B", "kB", "MB", "GB", "TB" };
					IConnection::DataCount data = connection->countData();
					return QString( "Fw: %1 B\nBk: %2 B" ).arg( data.first ).arg( data.second );
				}
				case SpeedColumn:
				case CommentColumn:
					return connection->recognizer()
						? connection->recognizer()->name() : "No suitable recognizer was found";
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
	connect( connection.data(), SIGNAL( packetArrived( IConnection::Pointer )),
		this, SLOT(updateConnection( IConnection::Pointer )) );

	const int pos = m_connections.count();

	beginInsertRows( QModelIndex(), pos, pos);
	m_connections.append( connection );
	endInsertRows();

	return true;
} 
/*----------------------------------------------------------------------------*/
bool ConnectionModel::updateConnection( const IConnection::Pointer connection, const Fields fields )
{
	Q_ASSERT (connection);
	
	const int i = m_connections.indexOf( connection );

	if ( i == -1 )
		return false;

	emit dataChanged( index( i, 0 ), index( i, COLUMNS - 1 ) );
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
	emit dataChanged( createIndex( 0, (int)ConnectionColumn ), 
		createIndex( m_connections.count(), (int)ConnectionColumn ) );
}
