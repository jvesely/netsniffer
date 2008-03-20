#include "QDebug"
#include "CConnection.h"

CConnection::CConnection() {
	reset();
	connect(&fuse, SIGNAL(timeout()), this, SLOT(timedout()));
}

CConnection::CConnection(const CConnection& connection) {
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
}
/*----------------------------------------------------------------------------*/
void CConnection::reset() {
	timeout = 15;
	protocol = DUMMY;
	addrSrc.clear();
	addrDest.clear();
	portSrc = 0;
	portDest = 0;
	dataForw.clear();
	dataBack.clear();
	countFr = 0;
	countBc = 0;
}
/*----------------------------------------------------------------------------*/
void CConnection::timedout() {
	// time exceeded I should die !!
	fuse.stop();
	reset();
	qDebug() << this->toString() << "Timed out!!" << endl ;
}
/*----------------------------------------------------------------------------*/
CConnection& CConnection::operator<<(const CPacket& packet) {

	qDebug() << QString::number(packet. hash(),16) << " former state of connection: " << toString() << endl;


	if (protocol == 0) { // first packet
		addrSrc = packet.srcAddress();
		addrDest = packet.destAddress();
		portSrc = packet.srcPort();
		portDest = packet.destPort();
		protocol = packet.trProtocol();
		++countFr;
		qDebug() << QString::number(packet. hash(),16) << " created connection: " << toString() << endl;
		return *this;
	}
	// my way
	if (protocol == packet.trProtocol() &&
			addrSrc == packet.srcAddress() &&
			addrDest == packet.destAddress() &&
			portSrc == packet.srcPort() &&
			portDest == packet.destPort()
			) {
		dataForw.append(packet.getData());
		++countFr;
		if (protocol == UDP)
			fuse.start(timeout); // open time window for UDP
	qDebug() << QString::number(packet. hash(),16) << " added fw packet" << toString() <<endl;

		return *this;
	}
	// return
	if (protocol == packet.trProtocol() &&
			addrSrc == packet.destAddress() &&
			addrDest == packet.srcAddress() &&
			portSrc == packet.destPort() &&
			portDest == packet.srcPort()
			) {
		dataBack.append(packet.getData());
		++countBc;
		if (protocol == UDP)
			fuse.start(timeout);
	qDebug() << QString::number(packet. hash(),16) << " added bc packet" <<endl;

		return *this;
	}
	
	return *this;
}
/*----------------------------------------------------------------------------*/
const QString CConnection::toString() const {
	QString ret("\nConnection:");
	ret.append((QString)"\nProtocol: " + ((protocol == UDP)?"UDP":"TCP"));
	ret.append("\nFrom: " + addrSrc.toString());
	ret.append(":" + QString::number(portSrc));
	ret.append(" (" + QString::number(countFr) + " packets)");
	ret.append("\nTo: " + addrDest.toString());
	ret.append(":" + QString::number(portDest));
	ret.append(" (" + QString::number(countBc) + " packets)");
	return ret;

}
