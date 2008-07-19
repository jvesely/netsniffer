#ifndef _CONNECTION_MODEL_H_
#define _CONNECTION_MODEL_H_
#include <QAbstractListModel>
#include <QVector>
#include "IConnection.h"

class ConnectionModel: public QAbstractListModel {

	Q_OBJECT

	QVector<IConnection *> store;

public:
	ConnectionModel() {};
	~ConnectionModel(){};
	int rowCount(const QModelIndex & parent = QModelIndex()) const;
	QVariant headerData ( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;
	QVariant data ( const QModelIndex & index, int role = Qt::DisplayRole ) const;

public slots:
	bool insertConnection(IConnection * conn);
	bool changeConnection(IConnection * conn);
	bool removeConnection(IConnection * conn);
};
#endif
