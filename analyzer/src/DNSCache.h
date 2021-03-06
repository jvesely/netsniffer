/*!
 * @file
 * @brief Short description.
 *
 * Long description. I would paste some Loren Ipsum rubbish here, but I'm afraid
 * It would stay that way. Not that this comment is by any means ingenious but
 * at least people can understand it.
 */
#pragma once

#include "IDNSCache.h"

class DNSCacheModel;

typedef QCache<QHostAddress, QString> AddressNameCache;
template class QCache<QHostAddress, QString>;

class DNSCache: public IDNSCache, private AddressNameCache
{
	Q_OBJECT;
	Q_DISABLE_COPY (DNSCache);

public:
	DNSCache() {};
	~DNSCache() {};

	const QString translate( const QHostAddress& address ) const;

	int maxEntries() const
		{ return this->AddressNameCache::maxCost(); }
	
	int countEntries() const
		{ return this->AddressNameCache::totalCost(); }

signals:
	void maxEntries( int );
	
	
public slots:
	bool insert( const QHostAddress& address, const QString& name );
	void setMaxEntries( int );

	friend class DNSCacheModel;
};

