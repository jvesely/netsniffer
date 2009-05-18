#pragma once

#include "Connection.h"

class IDNSCache;

typedef QVector<ConnectionPtr> ConnectionVector;
typedef QHash<Connection*, int> IndexHash;

class ConnectionModel: public QAbstractListModel
{
public:
  enum ConnectionField
  {
    Nothing = 0x0,
    Address = 0x1,
    PacketCount = 0x2,
    Speed = 0x4,
    Comment = 0x8,
    Status = 0x10,
    All = 0x20
  };

  Q_DECLARE_FLAGS (Fields, ConnectionField);

	ConnectionModel( const IDNSCache* dns );
	~ConnectionModel() { m_connections.clear(); };

	inline int rowCount(const QModelIndex & parent = QModelIndex()) const 
		{ Q_UNUSED(parent); return m_connections.count(); };

	inline int columnCount(const QModelIndex& parent = QModelIndex()) const 
		{ Q_UNUSED(parent); return COLUMNS; };

	inline ConnectionPtr connection( QModelIndex index ) const
		{ return (index.isValid() && index.row() < m_connections.count()) 
			? m_connections[ index.row() ] : ConnectionPtr( NULL ); }

	QVariant headerData ( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;
	
	QVariant data ( const QModelIndex& index, int role = Qt::DisplayRole ) const;

public slots:
	bool insertConnection( ConnectionPtr conn );
	bool updateConnection( ConnectionPtr conn,  Fields fields = All );
	bool removeConnection( ConnectionPtr conn );

	void DNSRefresh( const QHostAddress& address, const QString& name );
	void SpeedRefresh();

private:

	Q_OBJECT;

	mutable QReadWriteLock m_guard;
	const IDNSCache* m_dns;
	ConnectionVector m_connections;
	IndexHash m_indexes;

	static const int COLUMNS = 5;
	enum Columns { TypeColumn, AddressColumn, PacketsCountColumn, SpeedColumn, CommentColumn };

	Q_DISABLE_COPY( ConnectionModel );
};

Q_DECLARE_OPERATORS_FOR_FLAGS (ConnectionModel::Fields);
