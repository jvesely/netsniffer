#pragma once

#include "Connection.h"




class ConnectionModel: public QAbstractListModel {

	Q_OBJECT

	struct ConnDesc{
		QString Addresses;
		QString Packets;
		QString Speeds;
		QString Comments;

	};

	mutable QReadWriteLock guard;
	QVector<QPair<Connection *, ConnDesc> > store;
	const QIcon UDPIcon;
	const QIcon TCPIcon;
	static const int COLUMNS = 4;

	void updateConnectionInfo(const Connection * conn, ConnDesc& desc, ConnectionField field);

public:
	ConnectionModel():UDPIcon(":/net/UDP32.png"), TCPIcon(":/net/TCP32.png"){};
	~ConnectionModel(){store.clear();};

	inline int rowCount(const QModelIndex & parent = QModelIndex()) const 
			{ Q_UNUSED(parent); return store.count(); };
	inline int columnCount(const QModelIndex& parent = QModelIndex()) const 
			{ Q_UNUSED(parent); return COLUMNS; };
	inline Connection * connection(QModelIndex index) const
			{ if (index.isValid() && index.row() < store.count()) return store[index.row()].first; return NULL; }

	QVariant headerData ( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;
	QVariant data ( const QModelIndex & index, int role = Qt::DisplayRole ) const;

public slots:
	bool insertConnection(Connection * conn);
	bool changeConnection(Connection * conn, ConnectionField field);
	bool removeConnection(Connection * conn);
	bool removeConnection(QObject * corpse);
};
