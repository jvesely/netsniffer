#ifndef _RECOGNIZER_H_
#define _RECOGNIZER_H_

#include <QPluginLoader>
#include <QPair>
#include <QString>
#include "ARecognizerEngine.h"
#include "Connection.h"

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
	void load();
	void setFile(QString path);
	QPair<QString, bool> getStatus();
//	QPair<QString, QString> * quick(Connection * con);

signals:
	void	statusChanged(QPair<QString, bool> status);

};


#endif
