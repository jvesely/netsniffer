#include <QDebug>
#include <QTimerEvent>
#include "Connection.h"

#define SPEED_INTERVAL 1000 //1s
#define TIMEOUT_INTERVAL 5000

Connection::Connection(QCache<QHostAddress, QString> * dns_, bool death, const Packet& packet):
	dns(dns_),
	info(packet.networkInfo()),
	maxFw(DEFAULT_MAX),
	maxBc(DEFAULT_MAX),
	countFr(1),
	countBc(0),
	dead(false),
	killDead(death),
	speedDown(0)

	
{
	qRegisterMetaType<ConnectionField>("ConnectionField");
	srvSrc = QString::number(info.sourcePort);
	srvDest = QString::number(info.destinationPort);
	nameSrc = info.sourceIP.toString();
	nameDest = info.destinationIP.toString();
	dataForw.append(packet.data());
	speedUp = dataUp = dataForw.size();
	timeout = TIMEOUT_INTERVAL;		
	deathTimer = startTimer(timeout);
	speedTimer = startTimer(SPEED_INTERVAL);
	//qDebug() << this << ": Connection created with autoDeath: " << killDead;
}
/*----------------------------------------------------------------------------*/
Connection::~Connection() {
	if ( !dead ){
		killTimer(deathTimer);
		killTimer(speedTimer);
	}
	//qDebug() << this << ": My last message this is ";

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
	QString *  names = (NULL);
	nameSrc = (names = (*dns)[info.sourceIP])?(*names):info.sourceIP.toString();
	nameDest = (names = (*dns)[info.destinationIP])?(*names):info.destinationIP.toString();

	speedUp = dataUp;
	speedDown = dataDown;
	dataUp = dataDown = 0;
}
/*----------------------------------------------------------------------------*/
void Connection::timerEvent(QTimerEvent * event) {
	// time exceeded I should die !!
	if (event->timerId() == speedTimer){
		 countSpeed();
		 emit changed(this, Cf_Speed); // to force update
		 return;
	}
	if (event->timerId() == deathTimer) {
	//	qDebug() << this <<": Death timer out!!!";
		dead = true;
		if ( !killDead ){
			emit changed(this, Cf_All);
			killTimer(deathTimer); // no longer needed
			killTimer(speedTimer);
		} else
			purge();
		return;
	}
	qDebug() << "Unknown timer "<< event->timerId();
}
/*----------------------------------------------------------------------------*/
void Connection::setQuick(QPair<QString, QString> desc){
	shortDescFw = desc.first;
	shortDescBc =  desc.second;
	emit changed(this, Cf_Comment);
}
/*----------------------------------------------------------------------------*/
void Connection::purge(){

	if (!dead)
		return;
//	qDebug() << this <<": I'm about to die " ;
	emit timedOut(this);
	deleteLater();
//	reset();
//	emit timedOut(this);
}
/*----------------------------------------------------------------------------*/
void Connection::setAutoPurge(bool on){
	killDead = on;
	//qDebug() << this << ": Set connection autoPurge: " << killDead;
	purge();
}
/*----------------------------------------------------------------------------*/
Connection& Connection::operator<<(const Packet& packet) {
	dead = false;
	NetworkInfo other = packet.networkInfo();
	// my way
	
	if (info.protocol == other.protocol &&
			info.sourceIP == other.sourceIP &&
			info.destinationIP == other.destinationIP &&
			info.sourcePort == other.sourcePort &&
			info.destinationPort == other.destinationPort
			)
	{
		dataForw.append(packet.data());
		dataUp += packet.data().count();
		++countFr;
		while (dataForw.count() >= maxFw)
			dataForw.removeFirst();
		killTimer(deathTimer);
		deathTimer = startTimer(timeout); // reopen time window
	}else
// return
		if (info.protocol == other.protocol &&
				info.sourceIP == other.destinationIP &&
				info.destinationIP == other.sourceIP &&
				info.sourcePort == other.destinationPort &&
				info.destinationPort == other.sourcePort
				)
		{
			dataBack.append(packet.data());
			dataDown += packet.data().count();
			++countBc;
			while (dataBack.count() >= maxBc)
				dataBack.removeFirst();

			killTimer(deathTimer);
			deathTimer = startTimer(timeout);
		}

	return *this;
}

/*----------------------------------------------------------------------------*/
const QString Connection::toString() const {
	if (info.protocol == DUMMY)
		return QString();

	QString from("Connection %1: \nProtocol: %2 \nFrom: %3:%4 (%7 packets)  speed: %5 %6\n");
	QString to("To: %1:%2 (%5 packets) speed %3 %4\n");
	from = from.arg((dead?(" (DEAD) "):""), (info.protocol == TCP)?"TCP":"UDP", nameSrc, srvSrc, getSpeed(speedUp), shortDescFw).arg(countFr);
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

