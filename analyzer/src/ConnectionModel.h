#pragma once

#include "Connection.h"

class IDNSCache;

class ConnectionModel: public QAbstractListModel
{
	struct ConnDesc {
		QString Addresses;
		QString Packets;
		QString Speeds;
		QString Comments;
	};

typedef QPair<Connection *, ConnDesc> Description;
typedef QVector<Description > DescriptionVector;

public:
	ConnectionModel( const IDNSCache* dns );
	~ConnectionModel() { m_store.clear(); };

	inline int rowCount(const QModelIndex & parent = QModelIndex()) const 
		{ Q_UNUSED(parent); return m_store.count(); };

	inline int columnCount(const QModelIndex& parent = QModelIndex()) const 
		{ Q_UNUSED(parent); return COLUMNS; };

	inline Connection* connection( QModelIndex index ) const
		{ return (index.isValid() && index.row() < m_store.count()) 
			? m_store[index.row()].first : NULL; }

	QVariant headerData ( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;
	
	QVariant data ( const QModelIndex & index, int role = Qt::DisplayRole ) const;

public slots:
	bool insertConnection( Connection * conn );
	bool changeConnection( Connection * conn, uint fields );
	bool removeConnection( Connection * conn );
	bool removeConnection( QObject * corpse );

	void DNSrefresh( const QHostAddress address, const QString name );

private:

	Q_OBJECT;

	mutable QReadWriteLock m_guard;
	const IDNSCache* m_dns;

	enum Columns { TypeColumn, AddressColumn, PacketsCountColumn, SpeedColumn, CommentColumn };

	DescriptionVector m_store;
	static const int COLUMNS = 5;

	Q_DISABLE_COPY( ConnectionModel );
	
	void updateConnectionInfo( const Connection* conn, ConnDesc& desc, uint fields);
};
