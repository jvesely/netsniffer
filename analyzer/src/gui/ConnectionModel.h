#pragma once

#include "IConnection.h"
#include "IDNSCache.h"
#include "IAnalyzer.h"

typedef QList<IConnection::Pointer> ConnectionList;

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

	ConnectionModel( IAnalyzer* analyzer ): m_analyzer( analyzer )
		{ 
			Q_ASSERT (analyzer);
			m_dns = analyzer->dnsCache();
			qRegisterMetaType<QHostAddress>( "QHostAddress" );
			QObject::connect( m_dns, SIGNAL(newEntry( const QHostAddress&, const QString& )), this, SLOT(DNSRefresh()) );
			qRegisterMetaType<IConnection::Pointer>( "IConnection::Pointer" );
			QObject::connect( analyzer, SIGNAL(newConnection( IConnection::Pointer )),
				this, SLOT(insertConnection( IConnection::Pointer )) );
		};

	~ConnectionModel() { m_connections.clear(); };
	
	inline IConnection::Pointer connection( QModelIndex index ) const
		{ return (index.isValid() && index.row() < m_connections.count()) 
			? m_connections[ index.row() ] : IConnection::Pointer( NULL ); }

	int rowCount( const QModelIndex& parent = QModelIndex() ) const 
		{ Q_UNUSED(parent); return m_connections.count(); };

	int columnCount( const QModelIndex& parent = QModelIndex() ) const 
		{ Q_UNUSED(parent); return COLUMNS; };

	QVariant headerData( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;
	
	QVariant data( const QModelIndex& index, int role = Qt::DisplayRole ) const;

public slots:
	bool insertConnection( IConnection::Pointer connection );
	bool updateConnection( const IConnection::Pointer conn,  const Fields fields = All );
	bool removeConnection( IConnection::Pointer conn );

private slots:
	void DNSRefresh();

private:
	const QVariant networkData( const NetworkInfo& info, int role ) const;

	mutable QReadWriteLock m_guard;
	const IDNSCache* m_dns;
	IAnalyzer* m_analyzer;
	ConnectionList m_connections;

	static const int COLUMNS = 4;
	enum Columns { ConnectionColumn, PacketsCountColumn, SpeedColumn, CommentColumn };

	Q_DISABLE_COPY( ConnectionModel );
	Q_OBJECT;
};

Q_DECLARE_OPERATORS_FOR_FLAGS (ConnectionModel::Fields);
