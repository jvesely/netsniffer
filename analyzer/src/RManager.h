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
	QList<Recognizer * > recognizers;
	QSet<QString> registeredFiles;
	QSet<const ARecognizerEngine*> registeredEngines;

	RManager(const RManager& copy);
	const RManager& operator=(const RManager& copy);
	
	const ARecognizerEngine * getNext(const ARecognizerEngine * engine) const;

public:
	inline RManager(){};
	inline ~RManager(){ dropAll(); };
	QPointer<Recognizer> getRecognizer(int i);
	inline QPointer<Recognizer> operator[](int i) { return getRecognizer(i); };
	const QList<IRecognizer * > currentRecognizers();
	inline int count() const {return recognizers.count();};

public slots:
	bool addRecognizer(QString path);
	bool dropRecognizer(int i);
	bool dropAll();
	void clean(QObject* obj);
	void registerFile(QString path);
	void registerEngine(ARecognizerEngine * engine);
	inline void unregisterFile(QString path){ registeredFiles.remove(path); };
	inline void unregisterEngine(ARecognizerEngine * engine){ registeredEngines.remove(engine); };
	void process(Connection * conn) const;

signals:
	void recognizerAdded(IRecognizer *);
	void error(QString);
	void addDnsRecord(QHostAddress addr, QString name);

};
#endif

