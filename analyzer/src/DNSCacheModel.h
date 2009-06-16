#pragma once
#include "DNSCache.h"

class DNSCacheModel: public QAbstractListModel
{
public:
	DNSCacheModel( DNSCache* dns = NULL )
		:m_dns( dns ) {};
	
	~DNSCacheModel() {};

	bool setDNS( DNSCache* dns );

	int rowCount( const QModelIndex& parent = QModelIndex() ) const
		{ Q_UNUSED (parent); return m_dns ? m_dns->count() : 0; };

	int columnCount( const QModelIndex& parent = QModelIndex() ) const
	  { Q_UNUSED (parent); return 2; };

	QVariant headerData( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;

	QVariant data( const QModelIndex& index, int role = Qt::DisplayRole ) const;

private slots:
	void invalidate()
		{ m_cacheValid = false; reset(); };

private:
	Q_OBJECT
	Q_DISABLE_COPY (DNSCacheModel)

	DNSCache* m_dns;
	mutable QList<QHostAddress> m_addressCache;
	mutable bool m_cacheValid;
};
