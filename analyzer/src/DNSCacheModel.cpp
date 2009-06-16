#include "DNSCacheModel.h"

DNSCacheModel::DNSCacheModel( DNSCache* dns ): m_dns( dns )
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
	if (!m_dns || index.row() > m_dns->count() || index.column() > 1 || role != Qt::DisplayRole )
		return QVariant();
	
	updateCache();

	switch (index.column())
	{
		case 0:
			return m_addressCache.at( index.row() ).toString();
		case 1:
			return m_dns->translate( m_addressCache.at( index.row() ) );
	}
	return QVariant();
}
/*----------------------------------------------------------------------------*/
bool DNSCacheModel::updateCache() const
{
	if (!m_cacheValid)
	{
		m_addressCache = m_dns->keys();
		m_cacheValid = true;
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
//		qDebug() << "removing:" << row << m_addressCache[ row ] << m_addressCache;
		beginRemoveRows( QModelIndex(), row, row );
		m_dns->remove( m_addressCache.takeAt( row ) );
//		qDebug() << "removed" << m_addressCache;
		endRemoveRows();
	}
}
