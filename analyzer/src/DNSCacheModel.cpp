#include "DNSCacheModel.h"

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

	if (!m_cacheValid)
	{
		m_addressCache = m_dns->keys();
		m_cacheValid = true;
	}

	switch (index.column())
	{
		case 0:
			return m_addressCache.at( index.row() ).toString();
		case 1:
			return m_dns->translate( m_addressCache.at( index.row() ) );
	}
}
