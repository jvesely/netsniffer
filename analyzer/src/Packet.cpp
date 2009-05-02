#include "Packet.h"

#define DEBUG_TEXT "[ Packet ]: "
#include "debug.h"

#define FIRST_HALF 240 // 0xf0
#define UDP_LENGTH 8 // 8 bytes per UDP header
#define TCP_MIN_LENGTH 20
#define FIN_FLAG 1 // first from the right side in 13th byte

/*----------------------------------------------------------------------------*/
Packet::Packet( const QByteArray& src )
{
	m_last = true;
	parse( src );
}
/*----------------------------------------------------------------------------*/
bool Packet::parse( const QByteArray& src )
{
	if (src.size() < 20) throw std::runtime_error("Too short"); // compulsory IP header fields
	const char* data = src.data();
	quint8 tmpByte = data[0]; // first byte
	const quint8 ver = tmpByte >> 4; // IP version (first 4 bits)
	
	if (ver != 4) 
		throw std::runtime_error( "Bad IP version" );

	//number of 32bit words in IP header
	const quint8 headerLen = (tmpByte &(~FIRST_HALF)) * 4; // to bytes
	
	if (src.size() < headerLen)
		throw std::runtime_error( "Corrupted: packet < header" );

	// I might read further without going out of the  header but it's a waste..
	
	const int packetLength = qFromBigEndian(*(quint16*)(data + 2)); // packet length
	
	if (src.size() != packetLength){
		qDebug() << packetLength << src.size(); 
		throw std::runtime_error( "Something went wrong size mismatch" );
	}

	m_info.protocol =	(TrProtocol)data[9]; //protocol
	m_info.sourceIP = QHostAddress(qFromBigEndian(*(quint32*)(data + 12)));
	m_info.destinationIP = QHostAddress(qFromBigEndian(*(quint32*)(data + 16)));

	int protBegin = headerLen;

	switch (m_info.protocol) {
		case TCP: { // some tcp stuff
			if (packetLength < TCP_MIN_LENGTH + headerLen) 
				throw std::runtime_error( "Too short for TCP" );
			quint8 TCPsize  = (data[protBegin + 12] & FIRST_HALF) * 4; 
			// tcp header Length
			if (packetLength < (headerLen + TCPsize) ) 
				throw std::runtime_error( "Shorter than indicated" );
			m_info.sourcePort = qFromBigEndian(*(quint16*)(data + protBegin));
			m_info.destinationPort = qFromBigEndian(*(quint16*)(data + protBegin + 2));
			//ok size is already checked so I might use this:
			m_last = data[protBegin + 13] & FIN_FLAG; // this is fin packet
			m_load = src.right(packetLength - (headerLen + TCPsize) );
		}
			break;
		case UDP:
			if (packetLength < (headerLen + UDP_LENGTH) )
				throw std::runtime_error( "Too short for UDP" );
			m_info.sourcePort = qFromBigEndian(*(quint16*)(data + protBegin));
			m_info.destinationPort = qFromBigEndian(*(quint16*)(data + protBegin + 2));
			m_load = src.right(packetLength - (headerLen + UDP_LENGTH) );
			break;
		default:
			throw std::runtime_error("Unsupported protocol");
	}
	return true;
}
/*----------------------------------------------------------------------------*/
bool Packet::operator == ( const Packet& packet ) const
{
/*	bool ret =	(
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
	*/
	return m_info == packet.m_info;
}
/*----------------------------------------------------------------------------*/
//uint Packet::hash() const
//{
//	return qHash( info );
/*	return 	qHash(info.sourceIP.toIPv4Address()) ^ 
					qHash(info.destinationIP.toIPv4Address()) ^
					qHash(info.sourcePort) ^
					qHash(info.destinationPort) ^
					qHash((int)info.protocol); 
*/
//}
/*----------------------------------------------------------------------------*/
//uint qHash(const Packet &packet) {
//	return packet.hash();
//}
/*----------------------------------------------------------------------------*/




