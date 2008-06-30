#include <QAbstractListModel>
#include <QVector>
#include "CConnection.h"

class CConnectionModel: public QAbstractListModel {

	Q_OBJECT

	QVector<CConnection *> store;

public:
	CConnectionModel();
	~CConnectionModel(){};
	int rowCount(const QModelIndex & parent = QModelIndex()) const;
	QVariant headerData ( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;
	QVariant data ( const QModelIndex & index, int role = Qt::DisplayRole ) const;

public slots:
	bool insertConnection(CConnection * conn);
	bool changeConnection(CConnection * conn);
	bool removeConnection(CConnection * conn);
};
