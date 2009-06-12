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
            arg( m_dns.translate( info.sourceIP ) ).
            arg( m_dns.translate( info.destinationIP ) ).
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
	QReadLocker lock( &m_guard );
#ifdef WINDOWS
#pragma message ("some might have been deleted during locked period consider removing assert")
#else
#warning "some might have been deleted during locked period consider removing assert"
#endif
	Q_ASSERT (index.row() >= 0 && index.row() < m_connections.count());

	const ConnectionPtr connection = m_connections.at( index.row() );
	static const QIcon icons[] =
		{ QIcon( ":/net/TCP32.png" ), QIcon( ":/net/UDP32.png" ) };

	const NetworkInfo& info = connection->networkInfo();
	
	if (role == Qt::DisplayRole)	
		switch (index.column()) {
//			case TypeColumn: // first column: Type
//				return QVariant();
			case ConnectionColumn: // adresses or dns
				return networkData( info, role );
/*					QString("From: %1:%3\nTo: %2:%4").
						arg( m_dns->translate( info.sourceIP ) ).
						arg( m_dns->translate( info.destinationIP ) ).
						arg( info.sourcePort ).arg( info.destinationPort );
*/
			case PacketsCountColumn: //packets
				return QString("Fw: %1\nBk: %2").
					arg(connection->packetCountForward()).arg( connection->packetCountBack() );
			case SpeedColumn: //speed
				return QString("Fw: %1\nBk: %2").arg( connection->speedForward() ).arg( connection->speedBack() );
			case CommentColumn: //fourth column: comment
				return m_comments.value( connection.data(), UNKNOWN );
			default:
				Q_ASSERT( !"No Such column" );
		}

	if (role == Qt::DecorationRole && index.column() == ConnectionColumn) {
		return info.protocol == TCP ?  icons[0] : icons[1];
	}

	if (role == Qt::SizeHintRole && index.column() == ConnectionColumn) {
		return 250;
	}

	if (role == Qt::BackgroundRole)
	{
		const Connection::ConnectionStatus status = connection->status();
		switch (status)
		{
			case Connection::Dead:
				return QColor("#d4dfe6"); 
			case Connection::Closed:
				return QColor("#d4e6d6");
		}
	}
//	else
//		return Qt::white;

	return QVariant();
}
/*----------------------------------------------------------------------------*/
bool ConnectionModel::insertConnection( ConnectionPtr connection )
{
	connect( connection.data(), SIGNAL(statusChanged( ConnectionPtr )), 
		this, SLOT(updateConnection( ConnectionPtr )), Qt::DirectConnection );

	QWriteLocker lock( &m_guard );
	const int pos = m_connections.count();

	beginInsertRows( QModelIndex(), pos, pos);
	m_connections.append( connection );
	endInsertRows();

	return true;
} 
/*----------------------------------------------------------------------------*/
bool ConnectionModel::updateConnection( const ConnectionPtr connection, const Fields fields )
{
	Q_ASSERT (connection);
	QReadLocker lock( &m_guard );
	
	const int i = m_connections.indexOf( connection );

	if ( i == -1 )
		return false;

	emit dataChanged( index( i, 0 ), index( i, COLUMNS - 1 ) );
	return true;
}
/*----------------------------------------------------------------------------*/
bool ConnectionModel::removeConnection( ConnectionPtr corpse )
{
	QWriteLocker lock( &m_guard );

	const int i = m_connections.indexOf( corpse );
	if ( i == -1 )
		return false;

	beginRemoveRows( QModelIndex(), i, i );
	m_connections.remove( i );
	endRemoveRows();

	return true;
}
/*----------------------------------------------------------------------------*/
void ConnectionModel::DNSRefresh()
{
	QReadLocker lock( &m_guard );
	emit dataChanged( createIndex( 0, (int)ConnectionColumn ), 
		createIndex( m_connections.count(), (int)ConnectionColumn ) );
//	resizeColumnToContents( (int)AddressColumn );
}
