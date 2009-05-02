#include "Packet.h"

#define DEBUG_TEXT "[ Packet ]: "
#include "debug.h"

static const int FIRST_HALF = 240; // 0xf0
static const int IPV4 = 4; // ip identifier for IPv4
static const int IP_HEADER_LENGTH = 20; // 20 bytes minimum size

static const int UDP_HEADER_LENGTH = 8; // 8 bytes per UDP header
static const int TCP_HEADER_MIN_LENGTH = 20; // minimal tcp header size
static const int FIN_FLAG = 1; // first from the right side in 13th byte

/*----------------------------------------------------------------------------*/
Packet::Packet( const QByteArray& src )
{
	m_last = true;
	parse( src );
}
/*----------------------------------------------------------------------------*/
bool Packet::parse( const QByteArray& src )
{
	if (src.size() < IP_HEADER_LENGTH)
		throw std::runtime_error( "Too short for IP" ); // compulsory IP header fields
	const char* data = src.data();
	const quint8 tmpByte = data[0]; // first byte
	const quint8 ver = tmpByte >> 4; // IP version (first 4 bits)
	
	if (ver != IPV4) 
		throw std::runtime_error( QString( "Bad IP version: %1" ).arg( ver ).toStdString() );

	//number of 32bit words in IP header, second half of the first byte
	const quint8 ipheader_length = (tmpByte & (~FIRST_HALF) ) * 4; // to bytes
	
	if (src.size() < ipheader_length)
		throw std::runtime_error( "Corrupted: packet < indicated header" );

	// I might read further without going out of the  header but it's a waste..
	
	const int packet_length = qFromBigEndian(*(quint16*)(data + 2)); // packet length
	
	if (src.size() != packet_length)
	{
		PRINT_DEBUG << "packet Length: " << packet_length << "real size:" << src.size(); 
		throw std::runtime_error( "Something went wrong size mismatch" );
	}

	m_info.protocol =	(TrProtocol)data[9]; //protocol
	m_info.sourceIP = QHostAddress(qFromBigEndian(*(quint32*)(data + 12)));
	m_info.destinationIP = QHostAddress(qFromBigEndian(*(quint32*)(data + 16)));

	const int transport_protocol_start = ipheader_length;

	switch (m_info.protocol) {
		case TCP: { // some tcp stuff
			if (packet_length < TCP_HEADER_MIN_LENGTH + ipheader_length) 
				throw std::runtime_error( "Too short for TCP." );
			const quint8 TCPsize  = (data[transport_protocol_start + 12] & FIRST_HALF) * 4; 
			// tcp header Length
			if (packet_length < (ipheader_length + TCPsize) ) 
				throw std::runtime_error( "Shorter than indicated." );

			m_info.sourcePort = qFromBigEndian(*(quint16*)(data + transport_protocol_start));
			m_info.destinationPort = qFromBigEndian(*(quint16*)(data + transport_protocol_start + 2));
			//ok size is already checked so I might use this:
			m_last = data[transport_protocol_start + 13] & FIN_FLAG; // this is fin packet
			m_load = src.right(packet_length - (ipheader_length + TCPsize) );
			return true;
		}
		
		case UDP:
			if (packet_length < (ipheader_length + UDP_HEADER_LENGTH) )
				throw std::runtime_error( "Too short for UDP" );
			
			m_info.sourcePort = qFromBigEndian(*(quint16*)(data + transport_protocol_start));
			m_info.destinationPort = qFromBigEndian(*(quint16*)(data + transport_protocol_start + 2));
			m_load = src.right(packet_length - (ipheader_length + UDP_HEADER_LENGTH) );
			return true;

		default:
			throw std::runtime_error( "Unsupported transport protocol" );
	}
	return true;
}
