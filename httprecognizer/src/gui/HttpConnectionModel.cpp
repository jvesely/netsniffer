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
			
			const QHttpRequestHeader header = mConnections[index.internalId() - 1].dialogue().keys().at( index.row() ).first;
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
		return mConnections[parent.row()].dialogue().count();

	return false;
}
/*----------------------------------------------------------------------------*/
QModelIndex HttpConnectionModel::index( int row, int column, const QModelIndex & parent ) const
{
	if (!parent.isValid() && column == 0 && row < mConnections.count())
		return createIndex( row, 0, 0 );
	
	if (parent.isValid() && parent.row() < mConnections.count())
	{
		const int resource_count = mConnections[parent.row()].dialogue().count();
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
		PRINT_DEBUG( "count for parent:" << parent << "root count:" << mConnections.count() );
		return mConnections.count();
	}

	if (parent.row() < mConnections.count())
	{
		const int count =  mConnections[parent.row()].dialogue().count();
		PRINT_DEBUG( "count for parent:" << parent << count );
		return count;
	}

	PRINT_DEBUG( "UNKNOWN COUNT" << parent );

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
