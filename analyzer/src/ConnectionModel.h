#pragma once

#include "qtInc.h"

#include "Connection.h"

#define COLUMNS 4

class ConnectionModel: public QAbstractListModel {

	Q_OBJECT

	QVector<Connection *> store;

public:
	ConnectionModel() {};
	~ConnectionModel(){};
	inline int rowCount(const QModelIndex & parent = QModelIndex()) const 
			{ Q_UNUSED(parent); return store.count(); };
	inline int columnCount(const QModelIndex& parent = QModelIndex()) const 
			{ Q_UNUSED(parent); return COLUMNS; };

	QVariant headerData ( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;
	QVariant data ( const QModelIndex & index, int role = Qt::DisplayRole ) const;

public slots:
	bool insertConnection(Connection * conn);
	bool changeConnection(Connection * conn, ConnectionField field);
	bool removeConnection(Connection * conn);
	bool removeConnection(QObject * corpse);
};
