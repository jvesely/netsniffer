#ifndef _RMANAGER_H_
#define _RMANAGER_H_

#include <QPointer>
#include <QPair>
#include <QVector>
#include <QPluginLoader>
#include <QCache>
#include "Recognizer.h"

class Recognizer;
class RManager:public QObject {
	Q_OBJECT;
private:
	QList<QPointer<Recognizer> > recognizers;
	QCache<QHostAddress, QString> * dns;

	RManager(const RManager& copy);
	const RManager& operator=(const RManager& copy);

public:
	RManager();
	~RManager();
	QPointer<Recognizer> getRecognizer(int i);
	QPointer<Recognizer> operator[](int i);

public slots:
	bool addRecognizer(QString path);
	bool dropRecognizer(int i);
	bool dropAll();
	int count();
	bool setDNS(QCache<QHostAddress, QString> * dns);
	void clean(QObject* obj);

signals:
	void recognizerAdded(Recognizer *);
	void error(QString);

};
#endif

