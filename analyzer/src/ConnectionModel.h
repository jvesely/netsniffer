#pragma once

#include "Connection.h"
#include "DNSCache.h"
#include "CommentStore.h"

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

	ConnectionModel( const DNSCache& dns, const CommentStore& comments )
		:m_dns( dns ), m_comments( comments )// {};
		{ 
			qRegisterMetaType<QHostAddress>( "QHostAddress" );
			QObject::connect( &m_dns, SIGNAL(newEntry( const QHostAddress&, const QString& )), this, SLOT(DNSRefresh()) );
//			QObject::connect( &m_comments, SIGNAL(changed( const Connection* )), this, SLOT(updateConnection( const ConnectionPtr )) );
		};

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
	bool updateConnection( const ConnectionPtr conn,  const Fields fields = All );
	bool removeConnection( ConnectionPtr conn );

private slots:
	void DNSRefresh();

private:
	const QVariant networkData( const NetworkInfo& info, int role ) const;

	mutable QReadWriteLock m_guard;
	const DNSCache& m_dns;
	const CommentStore& m_comments;
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
