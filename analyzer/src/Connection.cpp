#include <QDebug>
#include <QTimerEvent>
#include "Connection.h"

#define speedint 1000

Connection::Connection() {
	dns = NULL;
	reset();
}

Connection::Connection(const Connection& connection):QObject(NULL) {
	timeout = 5000;
	addrSrc = connection.addrSrc;
	addrDest = connection.addrDest;
	portSrc = connection.portSrc;
	portDest = connection.portDest;
	protocol = connection.protocol;
	dataForw = connection.dataForw;
	dataBack = connection.dataBack;
	timeout = connection.timeout;
	countFr = connection.countFr;
	countBc = connection.countBc;
	dns = connection.dns;
	deathTimer = startTimer(timeout);
	speedTimer = startTimer(speedint);
	speedUp = connection.speedUp;
	speedDown = connection.speedDown;
	dataUp = connection.dataUp;
	dataDown = connection.dataDown;
	killDead = false;
}
/*----------------------------------------------------------------------------*/
void Connection::reset() {
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
	dns = NULL;
	killTimer(deathTimer);
	killTimer(speedTimer);
	speedUp = 0;
	speedDown = 0;
	dataUp = 0;
	dataDown = 0;
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
		 emit addedPacket(this); // to force update
	}
	if (event->timerId() == deathTimer) {
		dead = true;
		if (killDead){
			reset();
			emit timedOut(this);
		}
	}
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
		QString * names = NULL;
		if (dns){
			qDebug() << (*dns)[addrSrc];
			qDebug() << (*dns)[addrDest];

			nameSrc = (names = (*dns)[addrSrc])?(*names):addrSrc.toString();
			nameDest = (names = (*dns)[addrDest])?(*names):addrDest.toString();
		} 
		portSrc = packet.srcPort();
		portDest = packet.destPort();
		protocol = packet.trProtocol();
		srvSrc = QString::number(portSrc);
		srvDest = QString::number(portDest);
		++countFr;
		dataForw.append(packet.getData());
		dataUp += packet.getData().count();
		killTimer(deathTimer); // stop old timer
		deathTimer = startTimer(timeout); // start new
		emit addedPacket(this);
		return *this;
	}
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
//		if (protocol == UDP)
			killTimer(deathTimer);
			deathTimer = startTimer(timeout); // open time window
		emit addedPacket(this);
		return *this;
	}
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
	//	if (protocol == UDP)
			killTimer(deathTimer);
			deathTimer = startTimer(timeout);
		emit addedPacket(this);
		return *this;
	}
	
	return *this;
}
/*----------------------------------------------------------------------------*/
const QString Connection::toString() const {
	if (protocol == DUMMY)
		return QString();
	QString from("Connection %1: \nProtocol: %2 \nFrom: %3:%4 (%6 packets)  speed: %5\n");
	QString to("To: %1:%2 (%4 packets) speed %3 \n");
	from = from.arg((dead?(" (DEAD) "):""), (protocol == TCP)?"TCP":"UDP", nameSrc, srvSrc, getSpeed(speedUp)).arg(countFr);
	to = to.arg(nameDest, srvDest, getSpeed(speedDown)).arg(countBc);

	return from + to;
}
