#include <stdexcept>
#include "errors.h"
#include "Recognizer.h"

Recognizer::Recognizer(QString path, QCache<QHostAddress, QString> *  dns_):loader(path) {
	dns = dns_;
	control = NULL;
	engine = qobject_cast<ARecognizerEngine *>(loader.instance());
	if (! engine)
		throw std::runtime_error(ERR_INVALID_ENGINE);
}
/*----------------------------------------------------------------------------*/
Recognizer::~Recognizer(){
	delete control;
	delete engine;
	loader.unload();

}
