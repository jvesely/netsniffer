#pragma once

#include "IConnection.h"
#include "IDNSCache.h"
#include "IAnalyzer.h"
#include "struct/SafeHash.h"

typedef QList<IConnection::Pointer> ConnectionList;
typedef SafeHash<IConnection::Pointer, int> ConnectionIndex;


class ConnectionModel: public QAbstractListModel
{
public:
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
		{ Q_UNUSED(parent); return Column::COUNT; };

	QVariant headerData( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;
	
	QVariant data( const QModelIndex& index, int role = Qt::DisplayRole ) const;

public slots:
	bool insertConnection( IConnection::Pointer connection );
	bool updateConnection( const IConnection::Pointer connection );
	bool removeConnection( IConnection::Pointer connection );

private slots:
	void DNSRefresh();

private:
	const QVariant networkData( const NetworkInfo& info, int role ) const;
	const QVariant packetsData( const IConnection::Pointer connection, int role ) const;
	const QVariant speedData( const IConnection::Pointer connection, int role ) const;
	const QVariant commentData( const IConnection::Pointer connection, int role ) const;

	const IDNSCache* m_dns;
	IAnalyzer* m_analyzer;
	ConnectionList m_connections;
	ConnectionIndex m_index;

	struct Column {
		enum Columns { Network, PacketsCount, Speed, Comment };
		static const int COUNT = 4;
	};

	Q_DISABLE_COPY( ConnectionModel );
	Q_OBJECT;
};
