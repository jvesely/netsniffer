#ifndef _RMANAGER_H_
#define _RMANAGER_H_

#include <QPointer>
#include <QPair>
#include <QVector>
#include <QPluginLoader>
#include <QCache>
#include <QSet>
#include "Recognizer.h"
#include "Connection.h"

class RManager:public QObject {
	Q_OBJECT;
private:
	QList<QPointer<Recognizer> > recognizers;
	QCache<QHostAddress, QString> * dns;
	QSet<QString> registeredFiles;
	QSet<ARecognizerEngine*> registeredEngines;

	RManager(const RManager& copy);
	const RManager& operator=(const RManager& copy);
	
	ARecognizerEngine * getNext(const ARecognizerEngine * engine) const;

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
	void registerFile(QString path);
	void unregisterFile(QString path);
	void registerEngine(ARecognizerEngine *);
	void unregisterEngine(ARecognizerEngine *);
	void process(Connection * conn) const;

signals:
	void recognizerAdded(Recognizer *);
	void error(QString);
	void addDnsRecord(QHostAddress addr, QString name);

};
#endif

