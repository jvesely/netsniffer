#include "ConnectionModel.h"
#include <QFont>

#define COLUMN1 "Addresses"
#define COLUMN2 "Packets"
#define COLUMN3 "Speed (B/s)"
#define COLUMN4 "Comment"

//const QPixmap ConnectionModel::UDPIcon = QPixmap(":/net/UDP32.png");

/*----------------------------------------------------------------------------*/
QVariant ConnectionModel::headerData(int section, Qt::Orientation orientation, int role) const {
	Q_UNUSED(section);
	//qDebug() << "orientation " << orientation << " section " << section << "role" << role;
	//if (role == Qt::FontRole)
		//return QFont("Palatino", 12);
	if (role == Qt::DisplayRole) {
		if (orientation == Qt::Horizontal) //
			switch (section){
				case 0: return COLUMN1;
				case 1: return COLUMN2;
				case 2: return COLUMN3;
				case 3: return COLUMN4;
				default: return QVariant();
			}
		else{
			QVariant ret((int)store[section].first->networkInfo().protocol);
			qDebug() << "Want vertical header" <<  ret;
			
		}
	}
	return QVariant();

}
/*----------------------------------------------------------------------------*/
QVariant ConnectionModel::data( const QModelIndex & index, int role) const {
	if (index.row() < 0 || index.row() >= store.count())
		return QVariant(); // this should never happen
	Connection * myConn = store[index.row()].first;
	NetworkInfo info = myConn->networkInfo();
	const ConnDesc &desc = store[index.row()].second;
	
	if (role == Qt::DisplayRole)	
	switch (index.column()){
		case 0: // first column addresses
			//return "Addresses";
			return desc.Addresses;
		case 1: //second column packets
			//return "Packets";
			return desc.Packets;			
		case 2: //third column speed
	//		return "Speed";
			return desc.Speeds;
		case 3: //fourth column comment
	//		return "Comment";
			return desc.Comments;
		default: return QVariant();
	}
	if (role == Qt::SizeHintRole && index.column() == 0)
		return 250;
	if (role == Qt::DecorationRole && index.column() == 0)
		return (info.protocol == TCP)?TCPIcon:UDPIcon;
	if (role == Qt::BackgroundRole && myConn->getStatus() == Connection::Cs_Dead)
		return Qt::darkGray; 
	return QVariant();
}
/*----------------------------------------------------------------------------*/
bool ConnectionModel::insertConnection(Connection * conn) {
	int pos = store.count();
	beginInsertRows(QModelIndex(), pos, pos);
	ConnDesc info;
	updateConnectionInfo(conn, info, Cf_All);
	store.append(QPair<Connection*, ConnDesc>(conn, info));
	//connect(conn, SIGNAL(timedOut(Connection *)), this, SLOT(removeConnection(Connection *)) );
	connect(conn, SIGNAL(destroyed(QObject *)), this, SLOT(removeConnection(QObject*)) );
	connect(conn, SIGNAL(changed(Connection*, ConnectionField)), this, SLOT(changeConnection(Connection*, ConnectionField)));
	endInsertRows();
	return true;
} 
/*----------------------------------------------------------------------------*/
bool ConnectionModel::changeConnection(Connection * conn, ConnectionField field) {
	if ( !conn )
		return false;
	int count = store.count();
	for (int i = 0; i < count; ++i)
		if (store[i].first == conn){
//			qDebug() << "Changing connection " << i << " field " << field;
			updateConnectionInfo(conn, store[i].second, field);
			emit dataChanged(index(i, field), index(i, field));
			return true;
		}
	//int row = store.indexOf(conn);
//	emit dataChanged(index(row, field), index(row, field) );
	return false;
}
/*----------------------------------------------------------------------------*/
bool ConnectionModel::removeConnection(Connection * conn) {
	int count = store.count();
	for (int i = 0; i < count; ++i)
		if (store[i].first == conn){
			beginRemoveRows(QModelIndex(), i, i);
			disconnect(conn, 0, this, 0);
			store.remove(i);
			endRemoveRows();
			return true;
		}

	return false;
}
/*----------------------------------------------------------------------------*/
void ConnectionModel::updateConnectionInfo(const Connection * conn, ConnDesc& desc, ConnectionField field){
	if (! conn)
		return;
	NetworkInfo info = conn->networkInfo();
	switch (field){
		case Cf_Address:
			desc.Addresses = QString("From: %1:%3\nTo: %2:%4").arg(conn->sourceName()).arg(conn->destinationName()).arg(info.sourcePort).arg(info.destinationPort);
			break;
		case Cf_PacketCount:
			desc.Packets = QString("Fw: %1\nBc: %2").arg(conn->packetCountFw()).arg(conn->packetCountBc());
			break;
		case Cf_Speed:
			desc.Speeds = QString("Fw: %1\nBc: %2").arg(conn->speedFw()).arg(conn->speedBc());
			break;
		case Cf_Comment:
			desc.Comments = "foo";//QString("%1 \n%2").arg(deref.fwDesc(), deref.bcDesc());
			break;
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
}


bool ConnectionModel::removeConnection(QObject * corpse) {
	int count = store.count();
	for (int i = 0; i < count; ++i)
		if (store[i].first == corpse){
			beginRemoveRows(QModelIndex(), i, i);
			store.remove(i);
			endRemoveRows();
			return true;
		}

	return false;
}
// */
