#include <stdexcept>
#include "errors.h"
#include "Recognizer.h"

Recognizer::Recognizer(QString path, QCache<QHostAddress, QString> *  dns_):loader(path) {
	dns = dns_;
	QObject * instance = loader.instance();
	engine = qobject_cast<ARecognizerEngine *>(instance);
	if (! engine){
		delete instance; // whatever it is
	//	throw std::runtime_error(ERR_INVALID_ENGINE);
	}
	qDebug() << "Recognizer created...";
}
/*----------------------------------------------------------------------------*/
Recognizer::~Recognizer(){
	delete engine;
	loader.unload();
	qDebug() << "Recognizer destroyed...";
}
/*----------------------------------------------------------------------------*/
void Recognizer::unload(){
	qDebug() << "unload";
	if(loader.isLoaded()){
		delete engine;
		qDebug() << "unloaded" << loader.unload();
		emit statusChanged(QPair<QString, bool>(loader.fileName(), loader.isLoaded()));
	}
}
/*----------------------------------------------------------------------------*/
void Recognizer::setFile(QString path) {
	if (loader.isLoaded()){
		delete engine;
		loader.unload();
	}
	loader.setFileName(path);
	emit statusChanged(QPair<QString, bool>(loader.fileName(), loader.isLoaded()));

}
/*----------------------------------------------------------------------------*/
void Recognizer::load() {
	qDebug() << "load";
	if (!loader.isLoaded()){
		loader.load();
		QObject * inst = loader.instance();
		engine = qobject_cast<ARecognizerEngine *>(inst);
		emit statusChanged(QPair<QString, bool>(loader.fileName(), loader.isLoaded()));
		if (! engine){
			delete inst; //whatever it is
			//throw std::runtime_error(ERR_INVALID_ENGINE);
		}

	}
}
/*----------------------------------------------------------------------------*/
QPair<QString, bool> Recognizer::getStatus(){
	return QPair<QString, bool>(loader.fileName(), loader.isLoaded());
}
