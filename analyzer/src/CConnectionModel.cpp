#include "CConnectionModel.h"

CConnectionModel::CConnectionModel() {
}
/*----------------------------------------------------------------------------*/
int CConnectionModel::rowCount(const QModelIndex & parent) const {
	if (parent.isValid())
		return store.count();
	else
		return store.count();
}
/*----------------------------------------------------------------------------*/
QVariant CConnectionModel::headerData(int section, Qt::Orientation orientation, int role) const {
	Q_UNUSED(section);
	Q_UNUSED(orientation);
	Q_UNUSED(role);
	return QVariant("Header");
}
/*----------------------------------------------------------------------------*/
QVariant CConnectionModel::data( const QModelIndex & index, int role) const {
	CConnection * me = store[index.row()];
	if (me && role == Qt::DisplayRole)
		return QVariant(me->toString());
	return QVariant();
}
/*----------------------------------------------------------------------------*/
bool CConnectionModel::insertConnection(CConnection * conn) {
	int pos = store.count();
	beginInsertRows(QModelIndex(), pos, pos);
	store.append(conn);
	connect(conn, SIGNAL(timedOut(CConnection *)), this, SLOT(removeConnection(CConnection *)) );
	endInsertRows();
	return true;
}
/*----------------------------------------------------------------------------*/
bool CConnectionModel::changeConnection(CConnection * conn) {
	int row = store.indexOf(conn);
	emit dataChanged(index(row, 0), index(row, 0) );
	return true;
}
/*----------------------------------------------------------------------------*/
bool CConnectionModel::removeConnection(CConnection * conn) {
	int row = store.indexOf(conn);
	beginRemoveRows(QModelIndex(), row, row);
	disconnect(conn, 0, this, 0);
	store.remove(row);
	endRemoveRows();
	return true;
}
