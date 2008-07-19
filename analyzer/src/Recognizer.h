#ifndef _RECOGNIZER_H_
#define _RECOGNIZER_H_

#include <QPluginLoader>
#include <QPair>
#include <QString>
#include "ARecognizerEngine.h"

class Recognizer: public QObject {

	Q_OBJECT;
private:
	QPluginLoader loader;
	ARecognizerEngine * engine;
public:
	Recognizer(QString plugin);
	~Recognizer();
	ARecognizerEngine * getEngine();

public slots:
	void unload();
	bool load();
	void setFile(QString path);
	QPair<QString, bool> getStatus();

signals:
	void statusChanged(QPair<QString, bool> status);
	void error(QString text);
	void registerFile(QString path);
	void unregisterFile(QString path);
	void registerEngine(ARecognizerEngine *);
	void unregisterEngine(ARecognizerEngine *);

};


#endif
