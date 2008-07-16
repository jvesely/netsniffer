#ifndef _RECOGNIZER_H_
#define _RECOGNIZER_H_

#include <QPluginLoader>
#include <QPair>
#include <QString>
#include "Connection.h"
#include "ARecognizerEngine.h"

class Recognizer: public QObject {

	Q_OBJECT;
private:
	QPluginLoader loader;
	ARecognizerEngine * engine;
	QCache<QHostAddress, QString> * dns;
public:
	Recognizer(QString plugin, QCache<QHostAddress, QString>* dns_);
	~Recognizer();

public slots:
	void unload();
	bool load();
	void setFile(QString path);
	QPair<QString, bool> getStatus();
//	QPair<QString, QString> * quick(Connection * con);

signals:
	void	statusChanged(QPair<QString, bool> status);
	void error(QString text);
	void registerFile(QString path);
	void unregisterFile(QString path);

};


#endif
