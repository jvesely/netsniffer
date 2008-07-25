#pragma once

#include "AnalyzerInc.h"

#include "Recognizer.h"
#include "Connection.h"

class RManager:public QThread {
	Q_OBJECT;
private:
	QList<QPointer<Connection> > quickQeue;
	QSet<QPointer<Connection> > quickSet;
	QSemaphore semaphoreGuard;
	QMutex mutexGuard;
	QList<Recognizer * > recognizers;
	QSet<QString> registeredFiles;
	QSet<const ARecognizerEngine*> registeredEngines;

	RManager(const RManager& copy);
	const RManager& operator=(const RManager& copy);
	
	const ARecognizerEngine * getNext(const ARecognizerEngine * engine) const;
	void run();
public:
	inline RManager(){ start(); };
	inline ~RManager(){ dropAll(); terminate(); wait();  };
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
	void insertQuick(QPointer<Connection> conn);

signals:
	void recognizerAdded(IRecognizer *);
	void error(QString);
	void addDnsRecord(QHostAddress addr, QString name);

};

template <typename T>
uint qHash(QPointer<T> ptr ){
	return (quintptr)ptr.data();
}
