#include "ConnectionModel.h"
#include "IDNSCache.h"

#define DEBUG_TEXT "[ Connection Model ]:"
#include "debug.h"

#define SIZE_HINT 250

ConnectionModel::ConnectionModel( const IDNSCache* dns )
:m_dns( dns )
{
	Q_ASSERT( dns );
}
/*----------------------------------------------------------------------------*/
QVariant ConnectionModel::headerData(int section, Qt::Orientation orientation, int role) const
{

	static const QString names[] = { "Type", "Addresses", "Packets", "Speed (B/s)", "Comment" };
	if (role == Qt::DisplayRole) {
		if (orientation == Qt::Horizontal) {
			return names[section];
		} /*else {
			QVariant ret((int)m_store[section].first->networkInfo().protocol);
			PRINT_DEBUG << "Want vertical header" <<  ret;	
		} */
	}
	return QVariant();
}
/*----------------------------------------------------------------------------*/
QVariant ConnectionModel::data( const QModelIndex& index, int role) const
{
	QReadLocker lock( &m_guard );
#warning some might have been deleted during locked period consider removing assert
	Q_ASSERT (index.row() >= 0 && index.row() < m_connections.count());

	const ConnectionPtr connection = m_connections.at( index.row() );
	static const QIcon icons[] =
		{ QIcon( ":/net/TCP32.png" ), QIcon( ":/net/UDP32.png" ) };

	const NetworkInfo& info = connection->networkInfo();
	
	if (role == Qt::DisplayRole)	
		switch (index.column()) {
			case TypeColumn: // first column: Type
				return QVariant();
			case AddressColumn: // adresses or dns
				return 
					QString("From: %1:%3\nTo: %2:%4").
						arg( m_dns->translate( info.sourceIP ) ).
						arg( m_dns->translate( info.destinationIP ) ).
						arg( info.sourcePort ).arg( info.destinationPort );

			case PacketsCountColumn: //packets
				return QString("Fw: %1\nBk: %2").
					arg(connection->packetCountForward()).arg( connection->packetCountBack() );
			case SpeedColumn: //speed
				return QString("Fw: %1\nBk: %2").arg( connection->speedForward() ).arg( connection->speedBack() );
			case CommentColumn: //fourth column: comment
				return "FOO";
			default:
				Q_ASSERT( !"No Such column" );
		}

	if (role == Qt::DecorationRole && index.column() == TypeColumn) {
		return info.protocol == TCP ?  icons[0] : icons[1];
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
//	connect( connection.data(), SIGNAL(destroyed( QObject* )),
//		this, SLOT(removeConnection( QObject* )), Qt::DirectConnection );
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
bool ConnectionModel::updateConnection( ConnectionPtr connection, ConnectionModel::Fields fields )
{
	Q_ASSERT (connection);
	QReadLocker lock( &m_guard );
	
	const int i = m_connections.indexOf( connection );

	if ( i == -1 )
		return false;

	emit dataChanged( index( i, TypeColumn ), index( i, CommentColumn ) );
	return true;
}
/*----------------------------------------------------------------------------*/
//bool ConnectionModel::removeConnection( QObject* corpse )
//{
//	return removeConnection( (Connection*)corpse );
//}
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
void ConnectionModel::DNSRefresh( const QHostAddress address, const QString name )
{
	Q_UNUSED( address );
	Q_UNUSED( name );
	
	QReadLocker lock( &m_guard );

	emit dataChanged( createIndex( 0, (int)AddressColumn ), 
		createIndex( m_connections.count(), (int)AddressColumn ) );
}
/*----------------------------------------------------------------------------*/
void ConnectionModel::SpeedRefresh()
{
	QReadLocker lock( &m_guard );
	emit dataChanged( createIndex( 0, (int)SpeedColumn ),
		createIndex( m_connections.count(), (int)SpeedColumn ) );
}
