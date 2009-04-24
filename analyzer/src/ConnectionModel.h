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

typedef QVector<Connection* > ConnectionVector;

public:
	ConnectionModel( const IDNSCache* dns );
	~ConnectionModel() { m_connections.clear(); };

	inline int rowCount(const QModelIndex & parent = QModelIndex()) const 
		{ Q_UNUSED(parent); return m_connections.count(); };

	inline int columnCount(const QModelIndex& parent = QModelIndex()) const 
		{ Q_UNUSED(parent); return COLUMNS; };

	inline Connection* connection( QModelIndex index ) const
		{ return (index.isValid() && index.row() < m_connections.count()) 
			? m_connections[index.row()] : NULL; }

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

	ConnectionVector m_connections;
	static const int COLUMNS = 5;

	Q_DISABLE_COPY( ConnectionModel );
};
