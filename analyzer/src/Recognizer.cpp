#include <stdexcept>
#include "errors.h"
#include "Recognizer.h"

Recognizer::Recognizer(QString path, QCache<QHostAddress, QString> *  dns_):loader(path), engine(NULL) {
	dns = dns_;
	load();
	qDebug() << "Recognizer created...";
}
/*----------------------------------------------------------------------------*/
Recognizer::~Recognizer(){
	unload();
	qDebug() << "Recognizer destroyed...";
}
/*----------------------------------------------------------------------------*/
void Recognizer::unload(){
	qDebug() << "unload";
	if(loader.isLoaded()){
		delete engine;
		qDebug() << "unloaded" << loader.unload() << "\n" << loader.errorString();
		emit statusChanged(QPair<QString, bool>(loader.fileName(), loader.isLoaded()));
	}
}
/*----------------------------------------------------------------------------*/
void Recognizer::setFile(QString path) {
	unload();
	loader.setFileName(path);
	emit statusChanged(QPair<QString, bool>(loader.fileName(), loader.isLoaded()));

}
/*----------------------------------------------------------------------------*/
bool Recognizer::load() {
	qDebug() << "load";
	if (!loader.isLoaded()){
		loader.load();
		QObject * inst = loader.instance();
		qDebug() << loader.errorString();
		engine = qobject_cast<ARecognizerEngine *>(inst);
		if (! engine){
			qDebug() << "totok je v tej kniznici" <<  inst;
			delete inst; //whatever it is
			loader.unload();
			//throw std::runtime_error(ERR_INVALID_ENGINE);
		}
		emit statusChanged(QPair<QString, bool>(loader.fileName(), loader.isLoaded()));

	}
	return loader.isLoaded();
}
/*----------------------------------------------------------------------------*/
QPair<QString, bool> Recognizer::getStatus(){
	return QPair<QString, bool>(loader.fileName(), loader.isLoaded());
}
