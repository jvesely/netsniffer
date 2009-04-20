#include "ConnectionModel.h"

#define DEBUG_TEXT "[ Connection Model ]:"
#include "debug.h"

#define SIZE_HINT 250
//const QPixmap ConnectionModel::UDPIcon = QPixmap(":/net/UDP32.png");

/*----------------------------------------------------------------------------*/
QVariant ConnectionModel::headerData(int section, Qt::Orientation orientation, int role) const
{

	static const QString names[] = { "Addresses", "Packets", "Speed (B/s)", "Comment" };
	if (role == Qt::DisplayRole) {
		if (orientation == Qt::Horizontal) {
			return names[section];
		} else {
			QVariant ret((int)m_store[section].first->networkInfo().protocol);
			PRINT_DEBUG << "Want vertical header" <<  ret;	
		}
	}
	return QVariant();
}
/*----------------------------------------------------------------------------*/
QVariant ConnectionModel::data( const QModelIndex & index, int role) const
{
	QReadLocker lock( &m_guard );

	Q_ASSERT (index.row() >= 0 && index.row() < m_store.count());


	const Connection* myConn = m_store[index.row()].first;
	const ConnDesc &desc = m_store[index.row()].second;
	NetworkInfo info = myConn->networkInfo();
	
	if (role == Qt::DisplayRole)	
		switch (index.column()){
			case 0: // first column addresses
				return desc.Addresses;
			case 1: //second column packets
				return desc.Packets;			
			case 2: //third column speed
				return desc.Speeds;
			case 3: //fourth column comment
				return desc.Comments;
			default:
				Q_ASSERT( !"No Such column" );
		}

	if (role == Qt::SizeHintRole && index.column() == 0)
		return SIZE_HINT;

	if (role == Qt::DecorationRole && index.column() == 0)
		return (info.protocol == TCP) ? TCPIcon : UDPIcon;

	if (role == Qt::BackgroundRole && myConn->getStatus() == Connection::Cs_Dead)
		return Qt::darkGray; 
//	else
//		return Qt::white;

	return QVariant();
}
/*----------------------------------------------------------------------------*/
bool ConnectionModel::insertConnection( Connection* conn )
{
	int pos = m_store.count();
	ConnDesc info;

	updateConnectionInfo(conn, info, Cf_All);

	{
		QWriteLocker lock( &m_guard );

		beginInsertRows(QModelIndex(), pos, pos);
		m_store.append(QPair<Connection*, ConnDesc>(conn, info));
		endInsertRows();
	}
	
	connect( conn, SIGNAL(destroyed( QObject* )), this, SLOT(removeConnection( QObject* )), Qt::DirectConnection );
	connect( conn, SIGNAL(changed( Connection*, uint )), 
		this, SLOT(changeConnection( Connection*, uint )), Qt::DirectConnection );

	return true;
} 
/*----------------------------------------------------------------------------*/
bool ConnectionModel::changeConnection( Connection * conn, uint fields )
{
	Q_ASSERT (conn);
	QReadLocker lock( &m_guard );

	const int count = m_store.count();
	for (int i = 0; i < count; ++i)
		if (m_store[i].first == conn)
		{
			updateConnectionInfo( conn, m_store[i].second, fields );
			emit dataChanged( index( i, 0 ), index( i, 3 ) );
			return true;
		}
	//int row = store.indexOf(conn);
//	emit dataChanged(index(row, field), index(row, field) );
	return false;
}
/*----------------------------------------------------------------------------*/
bool ConnectionModel::removeConnection(Connection * conn)
{
	Q_ASSERT (!"FOO this one should not be used...");

	int count = m_store.count();
	for (int i = 0; i < count; ++i)
		if (m_store[i].first == conn){
			beginRemoveRows(QModelIndex(), i, i);
			disconnect(conn, 0, this, 0);
			m_store.remove(i);
			endRemoveRows();
			return true;
		}

	return false;
}
/*----------------------------------------------------------------------------*/
bool ConnectionModel::removeConnection( QObject * corpse )
{
	QWriteLocker lock( &m_guard );

	const int count = m_store.count();
	for (int i = 0; i < count; ++i)
		if (m_store[i].first == corpse)
		{
			beginRemoveRows( QModelIndex(), i, i );
			m_store.remove( i );
			endRemoveRows();
			return true;
		}

	return false;
}
/*----------------------------------------------------------------------------*/
void ConnectionModel::updateConnectionInfo( const Connection * conn, ConnDesc& desc, uint fields )
{
	Q_ASSERT (conn);

	NetworkInfo info = conn->networkInfo();
	if (fields & Cf_Address)
		desc.Addresses = QString("From: %1:%3\nTo: %2:%4").
			arg( info.sourceIP.toString() ).
			arg( info.destinationIP.toString() ).
			arg( info.sourcePort ).arg( info.destinationPort );
	
	if (fields & Cf_PacketCount)
		desc.Packets = QString("Fw: %1\nBc: %2").arg(conn->packetCountFw()).arg(conn->packetCountBc());

	if (fields & Cf_Speed)
		desc.Speeds = QString("Fw: %1\nBc: %2").arg(conn->speedFw()).arg(conn->speedBc());
		
	if (fields & Cf_Comment)
		desc.Comments = "foo";//QString("%1 \n%2").arg(deref.fwDesc(), deref.bcDesc());
/*			
		case Cf_Status:
			break;
		case Cf_All:
			desc.Addresses = QString("From: %1:%3\nTo: %2:%4").arg(conn->sourceName()).arg(conn->destinationName()).arg(info.sourcePort).arg(info.destinationPort);
			desc.Packets = QString("Fw: %1\nBc: %2").arg(conn->packetCountFw()).arg(conn->packetCountBc());
			desc.Speeds = QString("Fw: %1\nBc: %2").arg(conn->speedFw()).arg(conn->speedBc());
			desc.Comments = "foo";//QString("%1 \n%2").arg(deref.fwDesc(), deref.bcDesc());
			break;
		default:;
	};
*/
}
/*----------------------------------------------------------------------------*/
// */
