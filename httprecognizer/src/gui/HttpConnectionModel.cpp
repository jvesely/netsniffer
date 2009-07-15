/*!
 * @file
 * @brief Short description.
 *
 * Long description. I would paste some Loren Ipsum rubbish here, but I'm afraid
 * It would stay that way. Not that this comment is by any means ingenious but
 * at least people can understand it.
 */
#include "HttpConnectionModel.h"

#define DEBUG_TEXT "[ HttpConnectionModel ]:"
#include "debug.h"

QVariant HttpConnectionModel::data( const QModelIndex& index, int role ) const
{
	if (!index.isValid())
		return QVariant();

	switch (role)
	{
		case Qt::DisplayRole:
			if (! index.internalId())
			{
				const QString host = mConnections[index.row()].lastRequestHeader().value( "host" );
				return host.isEmpty() ? "Unknown host" : host;
			}
			
			const QHttpRequestHeader header = mConnections[index.internalId() - 1].session().at( index.row() ).first.first;
			return header.method() + " " + header.path();

			return "foo";
	}
	return QVariant();
}
/*----------------------------------------------------------------------------*/
QVariant HttpConnectionModel::headerData( int section, Qt::Orientation orientation, int role ) const
{
	if (role == Qt::DisplayRole && orientation == Qt::Horizontal && section == 0)
		return "Captured resources"; 
	return QVariant();
}
/*----------------------------------------------------------------------------*/
bool HttpConnectionModel::hasChildren( const QModelIndex& parent ) const
{
	if (!parent.isValid())
		return !mConnections.isEmpty();

	if (!parent.parent().isValid() && parent.row() < mConnections.count())
		return mConnections[parent.row()].session().count();

	return false;
}
/*----------------------------------------------------------------------------*/
QModelIndex HttpConnectionModel::index( int row, int column, const QModelIndex & parent ) const
{
	if (!parent.isValid() && column == 0 && row < mConnections.count())
		return createIndex( row, 0, 0 );
	
	if (parent.isValid() && parent.row() < mConnections.count())
	{
		const int resource_count = mConnections[parent.row()].session().count();
		if (column == 0 && row < resource_count)
			return createIndex( row, 0, parent.row() + 1 );
	}
	return QModelIndex();
}
/*----------------------------------------------------------------------------*/
QModelIndex HttpConnectionModel::parent( const QModelIndex& index ) const
{
	if (!index.isValid() || !index.internalId())
		return QModelIndex();

	const int parent_row = index.internalId();
	return createIndex( parent_row - 1, 0 );
}
/*----------------------------------------------------------------------------*/
int HttpConnectionModel::rowCount( const QModelIndex& parent ) const
{
	if (!parent.isValid())
	{
		return mConnections.count();
	}

	if (parent.row() < mConnections.count())
	{
		const int count =  mConnections[parent.row()].session().count();
		return count;
	}
	return 0;
}
/*----------------------------------------------------------------------------*/
bool HttpConnectionModel::addConnection( const HttpConnection connection )
{
	const int pos = mConnections.count();
	beginInsertRows( QModelIndex(), pos, pos);
	mConnections.append( connection );
	endInsertRows();
	return true;
}
/*----------------------------------------------------------------------------*/
const HttpConnection HttpConnectionModel::getConnection( const QModelIndex& index ) const
{
	if (!index.isValid() || index.internalId() || index.row() >= mConnections.count())
		return HttpConnection();

	return mConnections[ index.row() ];
}
