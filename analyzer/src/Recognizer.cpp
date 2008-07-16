#include <stdexcept>
#include "errors.h"
#include "Recognizer.h"

Recognizer::Recognizer(QString path, QCache<QHostAddress, QString> *  dns_):loader(path), engine(NULL) {
	dns = dns_;
	qDebug() << "Recognizer created...";
}
/*----------------------------------------------------------------------------*/
Recognizer::~Recognizer(){
	emit unregisterFile(loader.fileName());
	unload();
	qDebug() << "Recognizer destroyed...";
}
/*----------------------------------------------------------------------------*/
void Recognizer::unload(){
	qDebug() << "unload" << loader.fileName();
	if(loader.isLoaded()){
		delete engine;
		qDebug() << "unloaded" << loader.unload() << "\n" << loader.errorString();
			emit statusChanged(QPair<QString, bool>(loader.fileName(), loader.isLoaded()));
	}
}
/*----------------------------------------------------------------------------*/
void Recognizer::setFile(QString path) {
	if (path == loader.fileName())
		return;
	unload();
	emit unregisterFile(loader.fileName());
	loader.setFileName(path);
	emit registerFile(loader.fileName());
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
		} 
		emit statusChanged(QPair<QString, bool>(loader.fileName(), loader.isLoaded()));

	}

	return loader.isLoaded();
}
/*----------------------------------------------------------------------------*/
QPair<QString, bool> Recognizer::getStatus(){
	return QPair<QString, bool>(loader.fileName(), loader.isLoaded());
}
