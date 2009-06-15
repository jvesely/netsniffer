#include "DNSCache.h"

bool DNSCache::insert( const QHostAddress& address, const QString& name )
{
	if (address.isNull() || name.isEmpty())
		return false;

	QString* stored_name = new QString( name );
	if (!stored_name)
		return false;
	
	const bool success = AddressNameCache::insert( address, stored_name );
	
	if (success)
		emit newEntry( address, *stored_name );
	return success;
}
/*----------------------------------------------------------------------------*/
const QString DNSCache::translate( const QHostAddress& address ) const
{
	const QString* stored_name = object( address );
	return stored_name ? *stored_name : address.toString();
}
/*----------------------------------------------------------------------------*/
QVariant DNSCache::headerData( int section, Qt::Orientation orientation, int role ) const
{
	if (role != Qt::DisplayRole || orientation != Qt::Horizontal)
		return QVariant();

	static const QVariant headers[] = { "IP Address", "DNS Name" };
	Q_ASSERT ((uint)section < 2);
	return headers[section];
}
