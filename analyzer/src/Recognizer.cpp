#include "Recognizer.h"

Recognizer::Recognizer(QString path, QCache<QHostAddress, QString> *  dns_):loader(path) {
	dns = dns_;
	control = NULL;
	engine = NULL;
}
/*----------------------------------------------------------------------------*/
Recognizer::~Recognizer(){
	delete control;
	delete engine;
	loader.unload();

}
