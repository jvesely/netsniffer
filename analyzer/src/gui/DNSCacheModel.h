/*!
 * @file
 * @brief Short description.
 *
 * Long description. I would paste some Loren Ipsum rubbish here, but I'm afraid
 * It would stay that way. Not that this comment is by any means ingenious but
 * at least people can understand it.
 */
#pragma once
#include "DNSCache.h"

class DNSCacheModel: public QAbstractListModel
{
public:
	DNSCacheModel( DNSCache* dns = NULL );
	
	~DNSCacheModel() {};

	bool setDNS( DNSCache* dns );

	int rowCount( const QModelIndex& parent = QModelIndex() ) const
		{ Q_UNUSED (parent); return mDns ? mDns->count() : 0; };

	int columnCount( const QModelIndex& parent = QModelIndex() ) const
	  { Q_UNUSED (parent); return 2; };

	QVariant headerData( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;

	QVariant data( const QModelIndex& index, int role = Qt::DisplayRole ) const;

	void remove( QModelIndexList items );

private slots:
	void invalidate()
		{ mCacheValid = false; reset(); };

private:
	Q_OBJECT
	Q_DISABLE_COPY (DNSCacheModel)

	bool updateCache() const;

	DNSCache* mDns;
	mutable QList<QHostAddress> mAddressCache;
	mutable bool mCacheValid;
};
