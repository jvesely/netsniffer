#pragma once

#include "Connection.h"


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
	ConnectionModel():UDPIcon( ":/net/UDP32.png" ), TCPIcon( ":/net/TCP32.png" ) {};
	
	~ConnectionModel() { m_store.clear(); };

	inline int rowCount(const QModelIndex & parent = QModelIndex()) const 
		{ Q_UNUSED(parent); return m_store.count(); };

	inline int columnCount(const QModelIndex& parent = QModelIndex()) const 
		{ Q_UNUSED(parent); return COLUMNS; };

	inline Connection * connection(QModelIndex index) const
		{ return (index.isValid() && index.row() < m_store.count()) ? m_store[index.row()].first : NULL; }

	QVariant headerData ( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;
	
	QVariant data ( const QModelIndex & index, int role = Qt::DisplayRole ) const;

public slots:
	bool insertConnection( Connection * conn );
	bool changeConnection( Connection * conn, uint fields );
	bool removeConnection( Connection * conn );
	bool removeConnection( QObject * corpse );

private:

	Q_OBJECT

	mutable QReadWriteLock m_guard;

	DescriptionVector m_store;
	const QIcon UDPIcon;
	const QIcon TCPIcon;
	static const int COLUMNS = 4;

	void updateConnectionInfo( const Connection* conn, ConnDesc& desc, uint fields);
};
