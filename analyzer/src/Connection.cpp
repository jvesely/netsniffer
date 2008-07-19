#include <QDebug>
#include <QTimerEvent>
#include "Connection.h"

#define speedint 1000 //1s

Connection::Connection(QCache<QHostAddress, QString> * dns_, bool death, RManager* recs){
	dns = dns_;
	killDead = death;
	recognizers = recs;
	reset(true);
	maxBc = DEFAULT_MAX;
	maxFw = DEFAULT_MAX;
}
/*----------------------------------------------------------------------------*/
Connection::~Connection() {
	killTimer(deathTimer);
	killTimer(speedTimer);
}
/*----------------------------------------------------------------------------*/
void Connection::setRecognizers(RManager* rec){
	recognizers = rec;
}
/*----------------------------------------------------------------------------*/
Connection::Connection():dns(NULL), recognizers(NULL) {
	killDead = false;
	reset(true);
	maxFw = maxBc = DEFAULT_MAX;
}
/*----------------------------------------------------------------------------*/
void Connection::reset(bool start) {
	timeout = 5000;
	protocol = DUMMY;
	addrSrc.clear();
	addrDest.clear();
	portSrc = 0;
	portDest = 0;
	dataForw.clear();
	dataBack.clear();
	countFr = 0;
	countBc = 0;
	if (! start){
		killTimer(deathTimer);
		killTimer(speedTimer);
	}
	
	speedUp = 0;
	speedDown = 0;
	dataUp = 0;
	dataDown = 0;
	dead = false;
	lastRec = NULL;
}
/*----------------------------------------------------------------------------*/
void Connection::setCache(QCache<QHostAddress, QString>* cache) {
	Q_ASSERT(dns == NULL);
	dns = cache;
}
/*----------------------------------------------------------------------------*/
int Connection::packetCount() const throw() {
	return countFr + countBc;
}
/*----------------------------------------------------------------------------*/
QString Connection::getSpeed(int speed) const{
	qreal sp = speed;
	if (sp < 1024)
		return QString::number(speed) + " B/s";
	
	sp /= (qreal)1024;

	if (sp < 1024)
		return QString::number(sp) + " kB/s";
	
	sp /= (qreal) 1024;
	return QString::number(sp) + " MB/s";
	
}

/*----------------------------------------------------------------------------*/
void Connection::countSpeed() {
	speedUp = dataUp;
	speedDown = dataDown;
	dataUp = dataDown = 0;
}
/*----------------------------------------------------------------------------*/
void Connection::timerEvent(QTimerEvent * event) {
	// time exceeded I should die !!
	if (event->timerId() == speedTimer){
		 countSpeed();
		 emit changed(this); // to force update
	}
	if (event->timerId() == deathTimer) {
		dead = true;
		if (killDead){
			reset();
			emit timedOut(this);
		} else
			emit changed(this); // force refresh
	}
	//qDebug() << "Unknown timer "<< event->timerId();
}
/*----------------------------------------------------------------------------*/
void Connection::setQuick(QPair<QString, QString> desc){
	shortDescFw = desc.first;
	shortDescBc =  desc.second;
	emit changed(this);
}
/*----------------------------------------------------------------------------*/
void Connection::setLast(const ARecognizerEngine * engine){
	lastRec = (ARecognizerEngine *)engine;
}
/*----------------------------------------------------------------------------*/
const ARecognizerEngine * Connection::getLast() const{
	return lastRec;
}
/*----------------------------------------------------------------------------*/
void Connection::purge(){
	if (!dead)
		return;
	
	reset();
	emit timedOut(this);
}
/*----------------------------------------------------------------------------*/
void Connection::setAutoPurge(bool on){
	killDead = on;
	purge();
}
/*----------------------------------------------------------------------------*/
Connection& Connection::operator<<(const Packet& packet) {
	dead = false;
	if (protocol == 0) { // first packet
		addrSrc = packet.srcAddress();
		addrDest = packet.destAddress();
		Q_ASSERT(dns != NULL);
		portSrc = packet.srcPort();
		portDest = packet.destPort();
		protocol = packet.trProtocol();
		srvSrc = QString::number(portSrc);
		srvDest = QString::number(portDest);
		++countFr;
		dataForw.append(packet.getData());
		dataUp += packet.getData().count();
		deathTimer = startTimer(timeout); // start new death tick
		speedTimer = startTimer(speedint); // count speed
		//emit changed(this);
		//return *this;
	}else
	// my way
		if (protocol == packet.trProtocol() &&
				addrSrc == packet.srcAddress() &&
				addrDest == packet.destAddress() &&
				portSrc == packet.srcPort() &&
				portDest == packet.destPort()
				)
		{
			dataForw.append(packet.getData());
			dataUp += packet.getData().count();
			++countFr;
			killTimer(deathTimer);
			deathTimer = startTimer(timeout); // reopen time window
		//	emit changed(this);
		//	return *this;
		}else
	// return
			if (protocol == packet.trProtocol() &&
					addrSrc == packet.destAddress() &&
					addrDest == packet.srcAddress() &&
					portSrc == packet.destPort() &&
					portDest == packet.srcPort()
					)
			{
				dataBack.append(packet.getData());
				dataDown += packet.getData().count();
				++countBc;
					killTimer(deathTimer);
					deathTimer = startTimer(timeout);
				//emit changed(this);
				//return *this;
			}
	while (dataForw.count() >= maxFw)
		dataForw.removeFirst();
	while (dataBack.count() >= maxBc)
		dataBack.removeFirst();

  QString *  names = (NULL);
  nameSrc = (names = (*dns)[addrSrc])?(*names):addrSrc.toString();
  nameDest = (names = (*dns)[addrDest])?(*names):addrDest.toString();

	//emit changed(this);
	return *this;
}

/*----------------------------------------------------------------------------*/
const QString Connection::toString() const {
	if (protocol == DUMMY)
		return QString();

	QString from("Connection %1: \nProtocol: %2 \nFrom: %3:%4 (%7 packets)  speed: %5 %6\n");
	QString to("To: %1:%2 (%5 packets) speed %3 %4\n");
	from = from.arg((dead?(" (DEAD) "):""), (protocol == TCP)?"TCP":"UDP", nameSrc, srvSrc, getSpeed(speedUp), shortDescFw).arg(countFr);
	to = to.arg(nameDest, srvDest, getSpeed(speedDown), shortDescBc).arg(countBc);

	return from + to;
}
const QByteArray Connection::getDataForw() const {
#warning WARNING NOT WORKING 
	return dataForw.last();	
}
const QByteArray Connection::getDataBack() const {
#warning WARNING NOT WORKING
	return dataBack.last();
} 
const QByteArray Connection::getLastPacketFw() const {
	if (dataForw.isEmpty())
		return QByteArray();
	return dataForw.last();
}
const QByteArray Connection::getLastPacketBc() const {
	if (dataBack.isEmpty())
		return QByteArray();
	return dataBack.last();
}
const QHostAddress Connection::getAddrSrc() const {
	return addrSrc;
}
const QHostAddress Connection::getAddrDest() const {
	return addrDest;
}
const quint16 Connection::getPortSrc() const {
	return portSrc;
}
const quint16 Connection::getPortDest() const {
	return portDest;
}
const TrProtocol Connection::getProto() const {
	return protocol;
}
void Connection::setFwPacketCount(int count) {
	maxFw = count;
}
void Connection::setBcPacketCount(int count) {
	maxBc = count;
}
