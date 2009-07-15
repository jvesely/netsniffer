/*!
 * @file
 * @brief Short description.
 *
 * Long description. I would paste some Loren Ipsum rubbish here, but I'm afraid
 * It would stay that way. Not that this comment is by any means ingenious but
 * at least people can understand it.
 */
#pragma once

#include "HttpConnection.h"

class HttpConnectionModel: public QAbstractListModel
{
public:
	typedef QList<HttpConnection> HttpConnectionList;

	HttpConnectionModel() {};
	HttpConnectionModel( const HttpConnectionList& connections )
	: mConnections( connections )	{};

	bool addConnection( const HttpConnection connection );
	
	const HttpConnection getConnection( const QModelIndex& index ) const;

	int rowCount( const QModelIndex& parent = QModelIndex() ) const;

	bool hasChildren( const QModelIndex& parent = QModelIndex() ) const;

	QModelIndex index( int row, int column, const QModelIndex & parent = QModelIndex() ) const;

	QModelIndex parent( const QModelIndex& index ) const;

	QVariant data( const QModelIndex& index, int role = Qt::DisplayRole ) const;

	QVariant headerData( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;

private:
	Q_DISABLE_COPY (HttpConnectionModel);

	HttpConnectionList mConnections;
};
