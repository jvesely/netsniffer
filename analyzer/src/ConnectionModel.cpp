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
//			return QString("From: %1:%3\nTo: %2:%4").arg(myConn->sourceName(), myConn->destinationName()).arg(QString::number(info.sourcePort), QString::number(info.destinationPort));
		case 1: //second column packets
			//return "Packets";
			return desc.Packets;			
//			return QStringList("Fw: %1 \nBc: %2").arg(myConn->packetCountFw()).arg(myConn->packetCountBc());
		case 2: //third column speed
	//		return "Speed";
			return desc.Speeds;
//			return QString("Fw: %1 \nBc: %2").arg(QString::number(myConn->speedFw()), QString::number(myConn->speedBc()));
		case 3: //fourth column comment
	//		return "Comment";
			return desc.Comments;
//			return QString("%1 \n%2").arg(myConn->fwDesc(), myConn->bcDesc());
		default: return QVariant();
	}
	if (role == Qt::SizeHintRole && index.column() == 0)
		return 250;
	if (role == Qt::DecorationRole && index.column() == 0)
		return (info.protocol == TCP)?TCPIcon:UDPIcon;
	return QVariant();
}
/*----------------------------------------------------------------------------*/
bool ConnectionModel::insertConnection(Connection * conn) {
	int pos = store.count();
	beginInsertRows(QModelIndex(), pos, pos);
	ConnDesc info;
	updateConnectionInfo(conn, info, Cf_All);
	store.append(QPair<Connection*, ConnDesc>(conn, info));
	connect(conn, SIGNAL(timedOut(Connection *)), this, SLOT(removeConnection(Connection *)) );
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
	const	Connection& deref = (*conn);
	switch (field){
		case Cf_Address:
			desc.Addresses = QString("From: %1:%3\nTo: %2:%4").arg(deref.sourceName()).arg(deref.destinationName()).arg(deref.networkInfo().sourcePort).arg(deref.networkInfo().destinationPort);
			break;
		case Cf_PacketCount:
			desc.Packets = QString("Fw: %1\nBc: %2").arg(deref.packetCountFw()).arg(deref.packetCountBc());
			break;
		case Cf_Speed:
			desc.Speeds = QString("Fw: %1\nBc: %2").arg(deref.speedFw()).arg(deref.speedBc());
			break;
		case Cf_Comment:
			desc.Comments = QString("%1 \n%2").arg(deref.fwDesc(), deref.bcDesc());
			break;
		case Cf_All:
		default:
			desc.Addresses = QString("From: %1:%3\nTo: %2:%4").arg(deref.sourceName()).arg(deref.destinationName()).arg(deref.networkInfo().sourcePort).arg(deref.networkInfo().destinationPort);
			desc.Packets = QString("Fw: %1\nBc: %2").arg(deref.packetCountFw()).arg(deref.packetCountBc());
			desc.Speeds = QString("Fw: %1\nBc: %2").arg(deref.speedFw()).arg(deref.speedBc());
			desc.Comments = QString("%1 \n%2").arg(deref.fwDesc(), deref.bcDesc());
	};
}

/*
bool ConnectionModel::removeConnection(QObject * corpse) {
	qDebug() << "removing carcass " << corpse;
	int row = store.indexOf( ( Connection * )corpse);
	beginRemoveRows(QModelIndex(), row, row);
	store.remove(row);
	endRemoveRows();
	return true;
}
// */
