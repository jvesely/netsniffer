#ifndef _RMANAGER_H_
#define _RMANAGER_H_

#include <QPointer>
#include <QPair>
#include <QVector>
#include <QPluginLoader>
#include <QCache>
#include "ARecognizer.h"

class RManager:public QObject {
	Q_OBJECT
private:
	QVector<QPair<QPointer<ARecognizer>, QPluginLoader *> > recognizers;
	QCache<QHostAddress, QString> * dns;

	RManager(const RManager& copy);
	const RManager& operator=(const RManager& copy);

public:
	RManager();
	~RManager();
	QPointer<ARecognizer> getRecognizer(int i);
	QPointer<ARecognizer> operator[](int i);

public slots:
	bool addRecognizer(QString path);
	bool dropRecognizer(int i);
	bool setDNS(QCache<QHostAddress, QString> * dns);

signals:
	int recognizerAdded();

};
#endif

