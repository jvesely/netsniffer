#pragma once

#include "IDNSCache.h"

typedef QCache<QHostAddress, QString> AddressNameCache;
template class QCache<QHostAddress, QString>;

class DNSCache: public IDNSCache, private AddressNameCache
{
	Q_OBJECT;
	Q_DISABLE_COPY (DNSCache);

public:
	const QString translate( const QHostAddress& address ) const;
	DNSCache() {};
	~DNSCache() {};

public slots:
	bool insert( const QHostAddress& address, const QString& name );
};

