#include "ConnectionModel.h"

/*----------------------------------------------------------------------------*/
QVariant ConnectionModel::headerData(int section, Qt::Orientation orientation, int role) const {
	Q_UNUSED(section);
	Q_UNUSED(role);
	if (orientation = Qt::Horizontal) //
		return QVariant("Captured Connections"); // column name (there is only one column in this model)
	else
		return QVariant(); // no row descriptions
}
/*----------------------------------------------------------------------------*/
QVariant ConnectionModel::data( const QModelIndex & index, int role) const {
	if (index.row() < 0 || index.row() >= store.count())
		return QVariant(); // this should never happen
	Connection * myConn = store[index.row()];
	
	switch (index.column()){
		case 0: // first column
			switch (role){
				case Qt::DisplayRole: return myConn->toString();
				case Qt::DecorationRole:   // an icon should be passed here
				case Qt::EditRole: return QVariant();
				case Qt::ToolTipRole: return QVariant("Doubleclick to analyze this connection");
				case Qt::StatusTipRole: return QVariant("Captured connection");
				default:return QVariant();

			}
		case 1: //second column
			return QString("Packets Fw: %1\nPackets Bc %2").arg(myConn->packetCountFw()).arg(myConn->packetCountBc());
	}
	return QVariant();
}
/*----------------------------------------------------------------------------*/
bool ConnectionModel::insertConnection(Connection * conn) {
	int pos = store.count();
	beginInsertRows(QModelIndex(), pos, pos);
	store.append(conn);
	connect(conn, SIGNAL(timedOut(Connection *)), this, SLOT(removeConnection(Connection *)) );
	connect(conn, SIGNAL(changed(Connection*)), this, SLOT(changeConnection(Connection*)));
	endInsertRows();
	return true;
} 
/*----------------------------------------------------------------------------*/
bool ConnectionModel::changeConnection(Connection * conn) {
#warning will have to be changed to reflect multiple column view
	if ( !conn )
		return false;
	//qDebug () << "Changing connection ";
	int row = store.indexOf(conn);
	emit dataChanged(index(row, 0), index(row, 0) );
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
