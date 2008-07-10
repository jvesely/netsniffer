#include <stdexcept>
#include "RManager.h"


QPointer<Recognizer> RManager::getRecognizer(int i){
	if (i > recognizers.count() || i < 0)
		return NULL;
	return recognizers[i];
}
/*----------------------------------------------------------------------------*/
QPointer<Recognizer> RManager::operator[](int i) {
	return getRecognizer(i);
}
/*----------------------------------------------------------------------------*/
bool RManager::addRecognizer(QString path) {
	Recognizer * newRec(NULL);
	qDebug() << "Adding recognizer " << path;
	try {
		newRec = new Recognizer(path, dns);
	}catch(std::runtime_error e){
		emit error(e.what());
		return false;
	}catch(...){
		emit error("Unknown exception");
		return false;
	}
	recognizers.append(newRec);
	qDebug() << "Recognizers: " << recognizers.count();
	return true;
}
/*----------------------------------------------------------------------------*/
bool RManager::dropRecognizer(int i) {
	qDebug() << "Dropping recognizer " << i;
	if(i >= recognizers.count() || i < 0)
		return false;
	Recognizer * item = recognizers[i];
//	QPluginLoader *  remPlg = item.second;
//	ARecognizer * remRec = item.first;
	delete item;
	recognizers.remove(i);
	qDebug() << "removed recognizer " << i;
	 return true;
}
/*----------------------------------------------------------------------------*/
bool RManager::setDNS(QCache<QHostAddress, QString> * newdns){
	return (dns = newdns);
}
/*----------------------------------------------------------------------------*/
bool RManager::dropAll(){
	qDebug() << "Dropping all";
	int max = recognizers.count();
	int i;
	for(i = 0; i < max; ++i )
		dropRecognizer(0);
	qDebug() << "Count: " << recognizers.count();
	return recognizers.count() == 0;
}
/*----------------------------------------------------------------------------*/
RManager::RManager(){
}
RManager::~RManager(){
	dropAll();
}
