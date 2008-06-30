#include <QDebug>
#include <QTimerEvent>
#include "Connection.h"

Connection::Connection() {
	reset();
}

Connection::Connection(const Connection& connection) {
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
	timer = startTimer(timeout);
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
	killTimer(timer);
}
/*----------------------------------------------------------------------------*/
int Connection::packetCount() const throw() {
	return countFr + countBc;
}
/*----------------------------------------------------------------------------*/
void Connection::timerEvent(QTimerEvent * event) {
	// time exceeded I should die !!
	reset();
	emit timedOut(this);
}
/*----------------------------------------------------------------------------*/
Connection& Connection::operator<<(const Packet& packet) {

	if (protocol == 0) { // first packet
		addrSrc = packet.srcAddress();
		addrDest = packet.destAddress();
		portSrc = packet.srcPort();
		portDest = packet.destPort();
		protocol = packet.trProtocol();
		++countFr;
		killTimer(timer); // stop old timer
		timer = startTimer(timeout); // start new
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
		++countFr;
//		if (protocol == UDP)
			killTimer(timer);
			timer = startTimer(timeout); // open time window for UDP
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
		++countBc;
	//	if (protocol == UDP)
			killTimer(timer);
			timer = startTimer(timeout);
		emit addedPacket(this);
		return *this;
	}
	
	return *this;
}
/*----------------------------------------------------------------------------*/
const QString Connection::toString() const {
	if (protocol == DUMMY)
		return QString();
	QString ret("Connection:");
	ret.append((QString)"\nProtocol: " + ((protocol == UDP)?"UDP":"TCP"));
	ret.append("\nFrom: " + addrSrc.toString());
	ret.append(":" + QString::number(portSrc));
	ret.append(" (" + QString::number(countFr) + " packets)");
	ret.append("\nTo: " + addrDest.toString());
	ret.append(":" + QString::number(portDest));
	ret.append(" (" + QString::number(countBc) + " packets)");
	return ret;
}
