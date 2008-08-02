#include "ConnectionModel.h"
#include <QFont>

#define COLUMN1 "Addresses"
#define COLUMN2 "Packets"
#define COLUMN3 "Speed (B/s)"
#define COLUMN4 "Comment"

//const QIcon ConnectionModel::UDP = QIcon(":/net/UDP48.png");

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
			QVariant ret((int)store[section]->networkInfo().protocol);
			qDebug() << "Want vertical header" <<  ret;
			
		}
	}
	return QVariant();

}
/*----------------------------------------------------------------------------*/
QVariant ConnectionModel::data( const QModelIndex & index, int role) const {
	if (index.row() < 0 || index.row() >= store.count())
		return QVariant(); // this should never happen
	Connection * myConn = store[index.row()];
	
	NetworkInfo info = myConn->networkInfo();
	if (role == Qt::DisplayRole)	
	switch (index.column()){
		case 0: // first column addresses
			//return "Addresses";
			return QString("From: %1:%3\nTo: %2:%4").arg(myConn->sourceName(), myConn->destinationName()).arg(QString::number(info.sourcePort), QString::number(info.destinationPort));
		case 1: //second column packets
			//return "Packets";
			return QString("Fw: %1 \nBc: %2").arg(myConn->packetCountFw()).arg(myConn->packetCountBc());
		case 2: //third column speed
	//		return "Speed";
			return QString("Fw: %1 \nBc: %2").arg(QString::number(myConn->speedFw()), QString::number(myConn->speedBc()));
		case 3: //fourth column comment
	//		return "Comment";
			return QString("%1 \n%2").arg(myConn->fwDesc(), myConn->bcDesc());
		default: return QVariant();
	}
	if(role == Qt::DecorationRole && index.column() == 0)
		return (info.protocol == TCP)?QIcon(":/net/TCP32.png"):QIcon(":/net/UDP32.png");
	return QVariant();
}
/*----------------------------------------------------------------------------*/
bool ConnectionModel::insertConnection(Connection * conn) {
	int pos = store.count();
	beginInsertRows(QModelIndex(), pos, pos);
	store.append(conn);
	connect(conn, SIGNAL(timedOut(Connection *)), this, SLOT(removeConnection(Connection *)) );
	connect(conn, SIGNAL(changed(Connection*, ConnectionField)), this, SLOT(changeConnection(Connection*, ConnectionField)));
	endInsertRows();
	return true;
} 
/*----------------------------------------------------------------------------*/
bool ConnectionModel::changeConnection(Connection * conn, ConnectionField field) {
	if ( !conn )
		return false;
	int row = store.indexOf(conn);
	emit dataChanged(index(row, field), index(row, field) );
	return true;
}
/*----------------------------------------------------------------------------*/
bool ConnectionModel::removeConnection(Connection * conn) {
	int row = store.indexOf(conn);
	beginRemoveRows(QModelIndex(), row, row);
	disconnect(conn, 0, this, 0);
	store.remove(row);
	endRemoveRows();
	return true;
}
/*----------------------------------------------------------------------------*/
bool ConnectionModel::removeConnection(QObject * corpse) {
	qDebug() << "removing carcass " << corpse;
	int row = store.indexOf( ( Connection * )corpse);
	beginRemoveRows(QModelIndex(), row, row);
	store.remove(row);
	endRemoveRows();
	return true;
}
