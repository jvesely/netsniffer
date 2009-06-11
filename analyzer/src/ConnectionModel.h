#pragma once

#include "Connection.h"
#include "IDNSCache.h"

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
	
	inline ConnectionPtr connection( QModelIndex index ) const
		{ return (index.isValid() && index.row() < m_connections.count()) 
			? m_connections[ index.row() ] : ConnectionPtr( NULL ); }

	inline int rowCount(const QModelIndex & parent = QModelIndex()) const 
		{ Q_UNUSED(parent); return m_connections.count(); };

	inline int columnCount(const QModelIndex& parent = QModelIndex()) const 
		{ Q_UNUSED(parent); return COLUMNS; };

	QVariant headerData ( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;
	
	QVariant data ( const QModelIndex& index, int role = Qt::DisplayRole ) const;

public slots:
	bool insertConnection( ConnectionPtr conn );
	bool updateConnection( ConnectionPtr conn,  const Fields fields = All );
	bool removeConnection( ConnectionPtr conn );

	void DNSRefresh( const QHostAddress& address, const QString& name );
	void SpeedRefresh();

private:
	inline const QVariant connectionData( const NetworkInfo& info, int role ) const;

	mutable QReadWriteLock m_guard;
	const IDNSCache* m_dns;
	ConnectionVector m_connections;
	IndexHash m_indexes;

	static const int COLUMNS = 4;
	enum Columns { ConnectionColumn, PacketsCountColumn, SpeedColumn, CommentColumn };

	Q_DISABLE_COPY( ConnectionModel );
	Q_OBJECT;
};

Q_DECLARE_OPERATORS_FOR_FLAGS (ConnectionModel::Fields);

/*----------------------------------------------------------------------------*/
/* DEFINITIONS ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
inline const QVariant ConnectionModel::connectionData( const NetworkInfo& info, int role ) const
{
	static const QIcon icons[] =
	    { QIcon( ":/net/TCP32.png" ), QIcon( ":/net/UDP32.png" ) };

	switch (role)
	{
		case Qt::DisplayRole:
			return
          QString("From: %1:%3\nTo: %2:%4").
            arg( m_dns->translate( info.sourceIP ) ).
            arg( m_dns->translate( info.destinationIP ) ).
            arg( info.sourcePort ).arg( info.destinationPort );

		case Qt::DecorationRole:
			return info.protocol == TCP ?  icons[0] : icons[1];		

		case Qt::SizeHintRole:
			return 250;

	}
}
