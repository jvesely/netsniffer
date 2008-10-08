#include <QDebug>
#include <QTimerEvent>
#include "Connection.h"

#define TIMEOUT_INTERVAL 5000

#define MY_WAY(PACKET) \
	(info.protocol == PACKET.protocol &&\
	info.sourceIP == PACKET.sourceIP &&\
	info.destinationIP == PACKET.destinationIP &&\
	info.sourcePort == PACKET.sourcePort &&\
	info.destinationPort == PACKET.destinationPort )

#define BACK_WAY(PACKET) \
	(info.protocol == PACKET.protocol &&\
	info.sourceIP == PACKET.destinationIP &&\
	info.destinationIP == PACKET.sourceIP &&\
	info.sourcePort == PACKET.destinationPort &&\
	info.destinationPort == PACKET.sourcePort )

Connection::Connection(const Packet& packet):
	info(packet.networkInfo()),
	countFr(1),
	countBc(0),
	speedDown(0),
	dataDown(0),
	deathTimer(this)

{
	status = Cs_Alive;
	qRegisterMetaType<ConnectionField>("ConnectionField");
	nameSrc = info.sourceIP.toString();
	nameDest = info.destinationIP.toString();
	data.append(QPair<bool, QByteArray>(true, packet.data()));
	lastPacketForward = packet.data();
	speedUp = dataUp = lastPacketForward.size();
	timeout = TIMEOUT_INTERVAL;		
	deathTimer.setSingleShot(true);
	deathTimer.setInterval(timeout);
	if (info.protocol == UDP){

		connect(&deathTimer, SIGNAL(timeout()), this, SLOT(close()));
		connect(this, SIGNAL(restartTimer()), &deathTimer, SLOT(start()));
		deathTimer.start();
	}
}
/*----------------------------------------------------------------------------*/
Connection::~Connection() {
	emit destroyed(this);
	disconnect();
	QWriteLocker locker(&guard); // wait if something is by any chance inserting packet
}
/*----------------------------------------------------------------------------*/
/*QString Connection::getSpeed(int speed) const{
	qreal sp = speed;
	if (sp < 1024)
		return QString::number(speed) + " B/s";
	
	sp /= (qreal)1024;

	if (sp < 1024)
		return QString::number(sp) + " kB/s";
	
	sp /= (qreal) 1024;
	return QString::number(sp) + " MB/s";
	
}
// */
/*----------------------------------------------------------------------------*/
void Connection::update(const QCache<QHostAddress, QString> * dns ) {
	QString *  names = (NULL);
	QWriteLocker lock(&guard);
	if (dns){
		nameSrc = (names = dns->object(info.sourceIP))?(*names):info.sourceIP.toString();
		nameDest = (names = dns->object(info.destinationIP))?(*names):info.destinationIP.toString();
	
		if(names)
			emit changed(this, Cf_Address);
	}
	speedUp = dataUp;
	speedDown = dataDown;
	dataUp = dataDown = 0;
	emit changed(this, Cf_Speed); // to force redraw
}
/*----------------------------------------------------------------------------*/
void Connection::die() {
	qDebug() << "Connection dying";
	status = Cs_Dead;
	if (killDead)
		deleteLater();
	else
		emit changed(this, Cf_Status);
//	if (event->timerId() == speedTimer){
		//count speed
//		countSpeed();
//		emit changed(this, Cf_Speed); // to force update
//		return;
//	}
	/*if (event->timerId() == deathTimer ) {
		if (status == Cs_Alive ){
			status = Cs_Closed;

			return;
		}
		Q_ASSERT(status == Cs_Closed);
		status = Cs_Dead;
		emit changed(this, Cf_Status);

		if ( !killDead ){
			//emit changed(this, Cf_All);
	//		STOP_TIMER(deathTimer); // no longer needed
//			STOP_TIMER(speedTimer); //nothing will come

		} else
			deleteLater();
		return;
	}
	qDebug() << "Unknown timer "<< event->timerId()<< " DeathTimer = " << deathTimer << " status =  " << status;*/
}
/*----------------------------------------------------------------------------*/
void Connection::setQuick(QPair<QString, QString> desc){
	//shortDescFw = desc.first;
	//shortDescBc =  desc.second;
	//emit changed(this, Cf_Comment);
}
/*----------------------------------------------------------------------------*/
/*void Connection::purge(){

	if (status != Cs_Closed)
		return;
//	qDebug() << this <<": I'm about to die " ;
//	emit timedOut(this);
	deleteLater();
//	reset();
//	emit timedOut(this);
}*/
/*----------------------------------------------------------------------------*/
void Connection::setAutoPurge(bool on){
	killDead = on;
	qDebug()<< "Autopurge " << killDead;
	if (status == Cs_Dead && on)
		deleteLater();
}
/*----------------------------------------------------------------------------*/
Connection& Connection::operator<<(const Packet& packet) {
	status = Cs_Alive;
	NetworkInfo packetInfo = packet.networkInfo();
	guard.lockForWrite();
	if MY_WAY(packetInfo)
	{
		lastPacketForward = packet.data();
		data.append(QPair<bool, QByteArray>(true, lastPacketForward));

		dataUp += lastPacketForward.count();
		++countFr;
	}else
// return
		if BACK_WAY(packetInfo) 
		{
			lastPacketBack = packet.data();
			data.append(QPair<bool, QByteArray>(false, lastPacketBack));
			dataDown += lastPacketBack.count();
			++countBc;
		}

	if (info.protocol == TCP && packet.isLast())
		close();
	if (info.protocol == UDP)
		emit restartTimer();

	guard.unlock();
	emit changed(this, Cf_PacketCount);
	return *this;
}

void Connection::close(){
		status = Cs_Closed;
		emit changed(this, Cf_Status);
		deathTimer.disconnect();
		qDebug()<< "Closed connection..";
		connect(&deathTimer, SIGNAL(timeout()), this, SLOT(die()));
		deathTimer.start(TIMEOUT_INTERVAL);
}
/*----------------------------------------------------------------------------*/
/*
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
// */
