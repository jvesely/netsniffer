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
	try {
		newRec = new Recognizer(path, dns);
	}catch(...){
		return false;
	}
	recognizers.append(newRec);
	return true;
}
/*----------------------------------------------------------------------------*/
bool RManager::dropRecognizer(int i) {
	if(i >= recognizers.count() || i < 0)
		return false;
	Recognizer* item = recognizers[i];
//	QPluginLoader *  remPlg = item.second;
//	ARecognizer * remRec = item.first;
	delete item;
	recognizers.remove(i);
	 return true;
}
/*----------------------------------------------------------------------------*/
bool RManager::setDNS(QCache<QHostAddress, QString> * newdns){
	return (dns = newdns);
}
/*----------------------------------------------------------------------------*/
RManager::~RManager(){
	int max = recognizers.count();
	for(int i = 0; i < max; ++i );
		dropRecognizer(0);
}
/*----------------------------------------------------------------------------*/
RManager::RManager(){
}
