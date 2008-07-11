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
	if (path.isEmpty() )
		return false;
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
	connect(newRec, SIGNAL(destroyed(QObject *)), this, SLOT(clean(QObject *)));
	emit recognizerAdded(newRec);
	return true;
}
/*----------------------------------------------------------------------------*/
bool RManager::dropRecognizer(int i) {
	qDebug() << "Dropping recognizer " << i;
	if(i >= recognizers.count() || i < 0)
		return false;
	delete	recognizers.takeAt(i);
	qDebug() << "removed recognizer " << i;
	 return true;
}
/*----------------------------------------------------------------------------*/
bool RManager::setDNS(QCache<QHostAddress, QString> * newdns){
	return (dns = newdns);
}
/*----------------------------------------------------------------------------*/
bool RManager::dropAll(){
	int max = recognizers.count();
	qDebug() << "Dropping all "<< max;
	while (!recognizers.isEmpty())
		delete recognizers.takeFirst();
	qDebug() << "Count: " << recognizers.count();
	return true;
	return recognizers.count() == 0;
}
/*----------------------------------------------------------------------------*/
RManager::RManager(){
}
/*----------------------------------------------------------------------------*/
RManager::~RManager(){
	dropAll();
}
/*----------------------------------------------------------------------------*/
void RManager::clean(QObject * ptr) {
	recognizers.removeAt(recognizers.indexOf(QPointer<Recognizer>(qobject_cast<Recognizer*>(ptr))));
	qDebug() << "Count: " << recognizers.count();
}
/*----------------------------------------------------------------------------*/
int RManager::count() {
	return recognizers.count();
}
