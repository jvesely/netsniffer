#include "Packet.h"

#define FIRST_HALF 240 // 0xf0
#define UDP_LENGTH 8 // 8 bytes per UDP header
#define TCP_MIN_LENGTH 20

/*----------------------------------------------------------------------------*/
bool Packet::parse(const QByteArray src) {
	if (src.size() < 20) throw std::runtime_error("Too short"); // compulsory IP header fields
	const char * data = src.data();
	quint8 tmpByte = data[0]; // first byte
	quint8 ver = tmpByte >> 4; // IP version (first 4 bits)
	
	if (ver != 4) 
		throw std::runtime_error("Bad IP version");

	//number of 32bit words in IP header
	quint8 headerLen = (tmpByte &(~FIRST_HALF)) * 4; 
	
	if (src.size() < headerLen) throw std::runtime_error("Corrupted: packet < header");

	// I might read further without going out of the  header but it's a waste..
	
	int packetLength = qFromBigEndian(*(quint16*)(data + 2)); // packet length
	
	if (src.size() != packetLength){
		qDebug() << packetLength << src.size(); 
		throw std::runtime_error("Something went wrong size mismatch");
	}

	info.protocol =	(TrProtocol)data[9]; //protocol
//	qDebug () << info.protocol;	
	info.sourceIP = QHostAddress(qFromBigEndian(*(quint32*)(data + 12)));
	info.destinationIP = QHostAddress(qFromBigEndian(*(quint32*)(data + 16)));

	int protBegin = headerLen;

	switch (info.protocol) {
		case TCP: { // some tcp stuff
			if (packetLength < TCP_MIN_LENGTH + headerLen) 
				throw std::runtime_error("Too short");
			quint8 TCPsize  = (data[headerLen + 12] & FIRST_HALF) * 4; 
			// tcp header Length
			if (packetLength < (headerLen + TCPsize) ) 
				throw std::runtime_error("Too short");
			info.sourcePort = qFromBigEndian(*(quint16*)(data + protBegin));
			info.destinationPort = qFromBigEndian(*(quint16*)(data + protBegin + 2));
			//ok size is already checked so I might use this:
//			qDebug() << "Source:\n" << src;
			load = src.right(packetLength - (headerLen + TCPsize) );
//			qDebug() << "Load:\n" << load;
		}
			break;
		case UDP:
			if (packetLength < (headerLen + UDP_LENGTH) )
				throw std::runtime_error("Too short");
			info.sourcePort = qFromBigEndian(*(quint16*)(data + protBegin));
			info.destinationPort = qFromBigEndian(*(quint16*)(data + protBegin + 2));
//			qDebug() << "Source:\n" << src.toHex();
			load = src.right(packetLength - (headerLen + UDP_LENGTH) );
//			qDebug() << "Load:\n" << load.toHex();

			break;
		default:
			throw std::runtime_error("Unsupported protocol");
	}
	return true;
}
/*----------------------------------------------------------------------------*/
bool Packet::operator==(const Packet& packet) const {
	

	bool ret =	(
		info.protocol == packet.info.protocol &&			//protocol matches
		( (	//forward
				(info.sourceIP == packet.info.sourceIP) && 
				(info.destinationIP == packet.info.destinationIP) &&
				(info.sourcePort == packet.info.sourcePort) &&
				(info.destinationPort == packet.info.destinationPort)
			) || (			//return trip
				(info.sourceIP == packet.info.destinationIP) &&
				(info.destinationIP == packet.info.sourceIP) &&
				(info.sourcePort == packet.info.destinationPort) &&
				(info.destinationPort == packet.info.sourcePort)
		))
	);
	return ret;
}
/*----------------------------------------------------------------------------*/
uint Packet::hash() const {
	return 	qHash(info.sourceIP.toIPv4Address()) ^ 
					qHash(info.destinationIP.toIPv4Address()) ^
					qHash(info.sourcePort) ^
					qHash(info.destinationPort) ^
					qHash((int)info.protocol); 

}
/*----------------------------------------------------------------------------*/
uint qHash(const Packet &packet) {
	return packet.hash();
}
/*----------------------------------------------------------------------------*/




