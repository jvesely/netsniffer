#include "ConnectionModel.h"

ConnectionModel::ConnectionModel() {
}
/*----------------------------------------------------------------------------*/
int ConnectionModel::rowCount(const QModelIndex & parent) const {
	if (parent.isValid())
		return store.count();
	else
		return store.count();
}
/*----------------------------------------------------------------------------*/
QVariant ConnectionModel::headerData(int section, Qt::Orientation orientation, int role) const {
	Q_UNUSED(section);
	Q_UNUSED(orientation);
	Q_UNUSED(role);
	return QVariant("Header");
}
/*----------------------------------------------------------------------------*/
QVariant ConnectionModel::data( const QModelIndex & index, int role) const {
	Connection * me = store[index.row()];
	if (me && role == Qt::DisplayRole)
		return QVariant(me->toString());
	return QVariant();
}
/*----------------------------------------------------------------------------*/
bool ConnectionModel::insertConnection(Connection * conn) {
	int pos = store.count();
	beginInsertRows(QModelIndex(), pos, pos);
	store.append(conn);
	connect(conn, SIGNAL(timedOut(Connection *)), this, SLOT(removeConnection(Connection *)) );
	endInsertRows();
	return true;
}
/*----------------------------------------------------------------------------*/
bool ConnectionModel::changeConnection(Connection * conn) {
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
