#pragma once

#include "qtInc.h"
#include "IRecognizer.h"
#include "ARecognizerEngine.h"

class Recognizer:public IRecognizer
{

	Q_OBJECT;
private:
	QPluginLoader loader;
	ARecognizerEngine * engine;
public:
	Recognizer(QString plugin);
	~Recognizer();
	ARecognizerEngine * getEngine();
	inline const QString fileName () const { return loader.fileName(); };
	inline bool isLoaded() const {return loader.isLoaded(); };

public slots:
	void unload();
	bool load();
	void setFile(QString path);
	QPair<QString, bool> getStatus();

signals:
	void error(QString text);
	void registerFile(QString path);
	void unregisterFile(QString path);
	void registerEngine(ARecognizerEngine *);
	void unregisterEngine(ARecognizerEngine *);

};
