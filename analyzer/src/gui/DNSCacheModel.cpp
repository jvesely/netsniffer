/*!
 * @file
 * @brief Short description.
 *
 * Long description. I would paste some Loren Ipsum rubbish here, but I'm afraid
 * It would stay that way. Not that this comment is by any means ingenious but
 * at least people can understand it.
 */
#include "DNSCacheModel.h"

DNSCacheModel::DNSCacheModel( DNSCache* dns ): mDns( dns ), mCacheValid( false )
{
	connect( dns, SIGNAL(newEntry( const QHostAddress&, const QString& )), this, SLOT(invalidate()) );
}
/*----------------------------------------------------------------------------*/
QVariant DNSCacheModel::headerData( int section, Qt::Orientation orientation, int role ) const
{
  if (role != Qt::DisplayRole || orientation != Qt::Horizontal)
    return QVariant();

  static const QVariant headers[] = { "IP Address", "DNS Name" };
  Q_ASSERT ((uint)section < 2);
  return headers[section];
}
/*----------------------------------------------------------------------------*/
QVariant DNSCacheModel::data( const QModelIndex& index, int role ) const
{
	updateCache();

	if (!mDns || index.row() >= mDns->count() || index.column() > 1 || role != Qt::DisplayRole )
		return QVariant();
	

	switch (index.column())
	{
		case 0:
			return mAddressCache.at( index.row() ).toString();
		case 1:
			return mDns->translate( mAddressCache.at( index.row() ) );
	}
	return QVariant();
}
/*----------------------------------------------------------------------------*/
bool DNSCacheModel::updateCache() const
{
	if (!mCacheValid)
	{
		mAddressCache = mDns->keys();
		mCacheValid = true;
		return true;
	}
	return false;
}
/*----------------------------------------------------------------------------*/
void DNSCacheModel::remove( QModelIndexList items )
{
	updateCache();
	qSort( items.begin(), items.end() );
//	qDebug() << "REMOVING: " << items.count() << items;
	QListIterator<QModelIndex> it( items );
	it.toBack();
	while (it.hasPrevious())
	{
		const int row = it.previous().row();
//		qDebug() << "removing:" << row << mAddressCache[ row ] << mAddressCache;
		beginRemoveRows( QModelIndex(), row, row );
		mDns->remove( mAddressCache.takeAt( row ) );
//		qDebug() << "removed" << mAddressCache;
		endRemoveRows();
	}
}
