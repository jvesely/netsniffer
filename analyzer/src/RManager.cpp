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
	if (registeredFiles.contains(path)){
		emit error(QString(ERR_LOADED_PLUGIN).arg(spath));
		return false;
	}

	Recognizer * newRec(NULL);
	newRec = new Recognizer(path, dns);
	registerFile(path);

	connect(newRec, SIGNAL(registerFile(QString)), this, SLOT(registerFile(QString)));
	connect(newRec, SIGNAL(unregisterFile(QString)), this, SLOT(unregisterFile(QString)));
	connect(newRec, SIGNAL(registerEngine(ARecognizerEngine *)), this, SLOT(registerEngine(ARecognizerEngine *)));
	connect(newRec, SIGNAL(unregisterEngine(ARecognizerEngine *)), this, SLOT(unregisterEngine(ARecognizerEngine *)));


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
/*----------------------------------------------------------------------------*/
void RManager::registerFile(QString path) {
	if (registeredFiles.contains(path))
		emit error(QString(ERR_LOADED_PLUGIN).arg(path));
	else
		registeredFiles.insert(path);
	//qDebug() << "Registered" <<  registeredFiles;
}
/*----------------------------------------------------------------------------*/
void RManager::unregisterFile(QString path) {
//	path = QFile::symLinkTarget(path);
//	qDebug() << "Unregistering " << path;
	registeredFiles.remove(path);
//	qDebug() << registeredFiles;
}

void RManager::registerEngine(ARecognizerEngine * engine) {
	registeredEngines.insert(engine);
}
void RManager::unregisterEngine(ARecognizerEngine * engine) {
	registeredEngines.remove(engine);
}
ARecognizerEngine * RManager::getNext(ARecognizerEngine * current) {
	QSet<ARecognizerEngine*>::const_iterator it = registeredEngines.constFind(current);
	if (it == registeredEngines.constEnd() || ++it == registeredEngines.constEnd())
		return *registeredEngines.constBegin();
	
	return *it;
}
/*----------------------------------------------------------------------------*/
void RManager::process(Connection * con){
	if (registeredEngines.count() == 0){
		con->setQuick(QPair<QString, QString>(QString("No recognizers"), QString("No recognizers")));
		return;
	}
	ARecognizerEngine * myEngine = con->getLast();
	if (! myEngine){
		myEngine = getNext(NULL); // there must be at least one :)
		con->setLast(myEngine);
	}
	Q_ASSERT(myEngine);
	QPair<QString, QString> res = myEngine->quickLook(
		con->getDataForw(),
		con->getDataBack(),
		con->getAddrSrc(),
		con->getAddrDest(),
		con->getPortSrc(),
		con->getPortDest(),
		con->getProto()
	);
	if(res.first.isEmpty() && res.second.isEmpty()) { // I am unsuccessful
		con->setQuick(QPair<QString, QString>(QString("Unknown"), QString("Unknown")));
		con->setLast(getNext(myEngine));
	}
	else
		con->setQuick(res);

	//ARecognizerEngine * myEngine = 
}

