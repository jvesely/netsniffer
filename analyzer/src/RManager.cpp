#include "errors.h"
#include "RManager.h"

QPointer<Recognizer> RManager::getRecognizer(int i){
	if (i > recognizers.count() || i < 0)
		return NULL;
	return recognizers[i];
}
/*----------------------------------------------------------------------------*/
bool RManager::addRecognizer(QString spath) {
	if (spath.isEmpty() )
		return false;
	
	const QString path(QPluginLoader(spath).fileName()); // get real file to be loaded
	if (registeredFiles.contains(path)){
		emit error(QString(ERR_LOADED_PLUGIN).arg(spath));
		return false;
	}

	Recognizer * newRec = new Recognizer(path);
	registerFile(path);
	//file signals
	connect(newRec, SIGNAL(registerFile(QString)), this, SLOT(registerFile(QString)));
	connect(newRec, SIGNAL(unregisterFile(QString)), this, SLOT(unregisterFile(QString)));
	//engine signals
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
void RManager::clean(QObject * ptr) {
	recognizers.removeAt(recognizers.indexOf(QPointer<Recognizer>(qobject_cast<Recognizer*>(ptr))));
	qDebug() << "Count: " << recognizers.count();
}
/*----------------------------------------------------------------------------*/
const QList<IRecognizer *> RManager::currentRecognizers() {
	QList<Recognizer * >::iterator it = recognizers.begin();

	QList<IRecognizer * > list;
	for (;it != recognizers.end(); ++it)
		list.append(*it);
	return list;
}
/*----------------------------------------------------------------------------*/
void RManager::registerFile(QString path) {
	if (registeredFiles.contains(path))
		emit error(QString(ERR_LOADED_PLUGIN).arg(path));
	else
		registeredFiles.insert(path);
}
/*----------------------------------------------------------------------------*/
void RManager::registerEngine(ARecognizerEngine * engine) {
	qDebug() << "Registering engine "<<engine<<" and connecting dns signal";
	registeredEngines.insert(engine);
	connect(engine, SIGNAL(dnsRecord(QHostAddress, QString)), this, SIGNAL(addDnsRecord(QHostAddress, QString)));
}
/*----------------------------------------------------------------------------*/
const ARecognizerEngine * RManager::getNext(const ARecognizerEngine * current) const {
	QSet<const ARecognizerEngine *>::ConstIterator it = registeredEngines.constFind(current); // hm, it should accept constant *
	if (it == registeredEngines.constEnd() || ++it == registeredEngines.constEnd())
		return *registeredEngines.constBegin();
	
	return *it;
}
/*----------------------------------------------------------------------------*/
void RManager::insertQuick(QPointer<Connection> con){ // needs guarded pointer
	mutexGuard.lock(); //acquire mutex
	if (!quickSet.contains(con)) { // no yet sheduled
		quickSet.insert(con);
		quickQeue.append(con);
		semaphoreGuard.release(); //release one
		qDebug() << "Connections to quickLook" << quickSet.count() << quickQeue.count() << semaphoreGuard.available();
	}
	
	mutexGuard.unlock();
	return;
}
void RManager::run() {
	qDebug() << "Thread started";
	while (running) {
		semaphoreGuard.acquire();
		mutexGuard.lock();
		if (quickQeue.isEmpty()){ // nothing to process
			mutexGuard.unlock();
			continue;
		}
		QPointer<Connection> con = quickQeue.takeFirst(); // take
		qDebug() << "Processing connection: " << con;
		quickSet.remove(con);// remove from set
		mutexGuard.unlock();
		
		if (!con) // it has died in te mean time :(
			continue;

		if (registeredEngines.count() == 0){
			con->setQuick(QPair<QString, QString>(QString("No recognizers"), QString("No recognizers")));
			continue;
		}

		const ARecognizerEngine * myEngine = con->getLast();
		if (! registeredEngines.contains(myEngine)){ // points to something weird
			myEngine = getNext(NULL); // there must be at least one :)
			con->setLast(myEngine);
		}
		Q_ASSERT(myEngine);
		QPair<QString, QString> res = myEngine->quickLook(con);
		if(res.first.isEmpty() && res.second.isEmpty()) { // I am unsuccessful
			con->setQuick(QPair<QString, QString>(QString("Unknown"), QString("Unknown")));
			con->setLast(getNext(myEngine));
		}
		else
			con->setQuick(res);
	}
}


