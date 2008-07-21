#include <stdexcept>
#include "errors.h"
#include "Recognizer.h"

Recognizer::Recognizer(QString path):loader(path), engine(NULL) {
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
		emit unregisterEngine(engine);
		delete engine;
		qDebug() << "unloaded" << loader.unload() << "\n" << loader.errorString();
			emit statusChanged(this);
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
	emit statusChanged(this);

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
		}else
			emit registerEngine(engine);
		emit statusChanged(this);

	}

	return loader.isLoaded();
}
/*----------------------------------------------------------------------------*/
QPair<QString, bool> Recognizer::getStatus(){
	return QPair<QString, bool>(loader.fileName(), loader.isLoaded());
}
