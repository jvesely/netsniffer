#include "RManager.h"


QPointer<ARecognizer> RManager::getRecognizer(int i){
	if (i > recognizers.count() || i < 0)
		return NULL;
	return recognizers[i].first;
}
/*----------------------------------------------------------------------------*/
QPointer<ARecognizer> RManager::operator[](int i) {
	return getRecognizer(i);
}
/*----------------------------------------------------------------------------*/
bool RManager::addRecognizer(QString path) {
	QPluginLoader * newPlg = new QPluginLoader(path);
	if (! newPlg)
		return false;
	ARecognizer * newRec = qobject_cast<ARecognizer *>(newPlg->instance());
	if (! newRec) {
		delete newPlg;
		return false;
	}
	newRec->setDNS(dns);
	recognizers.append(QPair<QPointer<ARecognizer>, QPluginLoader* >(newRec, newPlg));
	return true;
}
/*----------------------------------------------------------------------------*/
bool RManager::dropRecognizer(int i) {
	if(i >= recognizers.count() || i < 0)
		return false;
	QPair<QPointer<ARecognizer>, QPluginLoader *> item = recognizers[i];
//	QPluginLoader *  remPlg = item.second;
//	ARecognizer * remRec = item.first;
	delete item.first;
	delete item.second;
	recognizers.remove(i);
	 return true;
}
/*----------------------------------------------------------------------------*/
bool RManager::setDNS(QCache<QHostAddress, QString> * newdns){
	return dns = newdns;
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
