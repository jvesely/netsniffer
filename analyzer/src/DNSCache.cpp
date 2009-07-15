/*!
 * @file
 * @brief Short description.
 *
 * Long description. I would paste some Loren Ipsum rubbish here, but I'm afraid
 * It would stay that way. Not that this comment is by any means ingenious but
 * at least people can understand it.
 */
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
void DNSCache::setMaxEntries( int cost )
{
	qDebug() << "Changing cache capacity" << cost;
	setMaxCost( cost );
	emit maxEntries( maxCost() );
}
