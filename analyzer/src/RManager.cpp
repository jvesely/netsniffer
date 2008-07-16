#include "errors.h"
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
bool RManager::addRecognizer(QString spath) {
	if (spath.isEmpty() )
		return false;
	const QString path(QPluginLoader(spath).fileName());
	if (registered.contains(path)){
		emit error(QString(ERR_LOADED_PLUGIN).arg(spath));
		return false;
	}

	Recognizer * newRec(NULL);
	newRec = new Recognizer(path, dns);
	registerFile(path);

	connect(newRec, SIGNAL(registerFile(QString)), this, SLOT(registerFile(QString)));
	connect(newRec, SIGNAL(unregisterFile(QString)), this, SLOT(unregisterFile(QString)));
	recognizers.append(newRec);
	connect(newRec, SIGNAL(destroyed(QObject *)), this, SLOT(clean(QObject *)));
	emit recognizerAdded(newRec);
	newRec->load();
	return true;
}
/*----------------------------------------------------------------------------*/
bool RManager::dropRecognizer(int i) {
	qDebug() << "Dropping recognizer " << i;
	if(i >= recognizers.count() || i < 0)
		return false;
	delete	recognizers.takeAt(i);
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
void RManager::registerFile(QString path) {
	if (registered.contains(path))
		emit error(QString(ERR_LOADED_PLUGIN).arg(path));
	else
		registered.insert(path);
	qDebug() << "Registered" <<  registered;
}
void RManager::unregisterFile(QString path) {
//	path = QFile::symLinkTarget(path);
	qDebug() << "Unregistering " << path;
	registered.remove(path);
	qDebug() << registered;
}
