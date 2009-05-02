#include "Packet.h"

#define DEBUG_TEXT "[ Packet ]: "
#include "debug.h"

static const int FIRST_HALF = 240; // 0xf0
static const int IPV4 = 4; // ip identifier for IPv4
static const int IP_HEADER_LENGTH = 20; // 20 bytes minimum size
static const int IP_SOURCE_ADDRESS_POS = 12;
static const int IP_DESTINATION_ADDRESS_POS = 16;
static const int UDP_HEADER_LENGTH = 8; // 8 bytes per UDP header
static const int TCP_HEADER_MIN_LENGTH = 20; // minimal tcp header size
static const int FIN_FLAG = 1; // first from the right side in 13th byte

/*----------------------------------------------------------------------------*/
Packet::Packet( const QByteArray& src )
{
	m_last = true;
	QString error;
	if (!parse( src, error ))
		throw std::runtime_error( error.toStdString() );
}
/*----------------------------------------------------------------------------*/
bool Packet::parse( const QByteArray& src, QString& error ) throw()
{
	if (src.size() < IP_HEADER_LENGTH)
		return error = "Too short for IP", false;
	
	const char* data = src.data();
	const quint8 tmpByte = data[0]; // first byte
	const quint8 version = tmpByte >> 4; // IP version (first 4 bits)
	
	if (version != IPV4)
		return error = QString( "Bad IP version: %1" ).arg( version ), false;

	//number of 32bit words in IP header, second half of the first byte
	const quint8 ipheader_length = (tmpByte & (~FIRST_HALF) ) * 4; // to bytes
	
	if (src.size() < ipheader_length)
		return error = "Corrupted: packet < indicated header", false;
	
	const int packet_length = qFromBigEndian(*(quint16*)(data + 2)); // packet length
	
	// I might read further without going out of the  header but it would be a waste..
	if (src.size() != packet_length)
		return error = QString( "Something went wrong size mismatch: %1 vs. %2" ).arg( packet_length ).arg( src.size() ), false;
	
	m_info.protocol =	(TrProtocol)data[9]; //protocol
	m_info.sourceIP = QHostAddress(qFromBigEndian(*(quint32*)(data + 12)));
	m_info.destinationIP = QHostAddress(qFromBigEndian(*(quint32*)(data + 16)));

	const int transport_protocol_start = ipheader_length;

	switch (m_info.protocol) {
		case TCP: { // some tcp stuff
			if (packet_length < TCP_HEADER_MIN_LENGTH + ipheader_length) 
				return error = "Too short for TCP.", false;
			
			const quint8 TCPsize  = (data[transport_protocol_start + 12] & FIRST_HALF) * 4; 
			// tcp header Length
			if (packet_length < (ipheader_length + TCPsize) )
				return error = "Shorter than indicated.", false;

			m_info.sourcePort = qFromBigEndian(*(quint16*)(data + transport_protocol_start));
			m_info.destinationPort = qFromBigEndian(*(quint16*)(data + transport_protocol_start + 2));
			//ok size is already checked so I might use this:
			m_last = data[transport_protocol_start + 13] & FIN_FLAG; // this is fin packet
			m_load = src.right(packet_length - (ipheader_length + TCPsize) );
			return true;
		}
		
		case UDP:
			if (packet_length < (ipheader_length + UDP_HEADER_LENGTH) )
				return error = "Too short for UDP", false;
			
			m_info.sourcePort = qFromBigEndian(*(quint16*)(data + transport_protocol_start));
			m_info.destinationPort = qFromBigEndian(*(quint16*)(data + transport_protocol_start + 2));
			m_load = src.right(packet_length - (ipheader_length + UDP_HEADER_LENGTH) );
			return true;

		default:
			return error = "Unsupported transport protocol", false;
	}
	return error = "Should never see me", false;
}
