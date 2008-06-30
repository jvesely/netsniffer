#include <QAbstractListModel>
#include <QVector>
#include "Connection.h"

class ConnectionModel: public QAbstractListModel {

	Q_OBJECT

	QVector<Connection *> store;

public:
	ConnectionModel();
	~ConnectionModel(){};
	int rowCount(const QModelIndex & parent = QModelIndex()) const;
	QVariant headerData ( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;
	QVariant data ( const QModelIndex & index, int role = Qt::DisplayRole ) const;

public slots:
	bool insertConnection(Connection * conn);
	bool changeConnection(Connection * conn);
	bool removeConnection(Connection * conn);
};
