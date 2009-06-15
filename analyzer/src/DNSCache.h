#pragma once

#include "IDNSCache.h"

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
	
	int rowCount( const QModelIndex& parent = QModelIndex() ) const
		{ Q_UNUSED (parent); return count(); };
	
	int columnCount( const QModelIndex& parent = QModelIndex() ) const
		{ Q_UNUSED (parent); return 2; };

	QVariant headerData( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;

	QVariant data( const QModelIndex& index, int role = Qt::DisplayRole ) const;
		

public slots:
	bool insert( const QHostAddress& address, const QString& name );
};

