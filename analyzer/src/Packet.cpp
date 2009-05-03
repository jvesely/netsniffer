#include "Packet.h"
#include "headers.h"

#define DEBUG_TEXT "[ Packet ]: "
#include "debug.h"

static const int FIRST_HALF = 240; // 0xf0
static const int IPV4 = 4; // ip identifier for IPv4
static const int IP_HEADER_LENGTH = 20; // 20 bytes minimum size
static const int IP_PACKET_LENGTH_POS = 2;
static const int IP_TRANSPORT_PROTOCOL_POS = 9;
static const int IP_SOURCE_ADDRESS_POS = 12;
static const int IP_DESTINATION_ADDRESS_POS = 16;
static const int UDP_HEADER_LENGTH = 8; // 8 bytes per UDP header
static const int UDP_SOURCE_PORT_POS = 0;
static const int UDP_DESTINATION_PORT_POS = 2;
static const int TCP_HEADER_MIN_LENGTH = 20; // minimal tcp header size
static const int TCP_HEADER_LENGTH_POS = 12;
static const int TCP_SOURCE_PORT_POS = 0;
static const int TCP_DESTINATION_PORT_POS = 2;
static const int TCP_FLAGS_POS = 13;
static const int TCP_FIN_FLAG = 1; // first from the right side in 13th byte

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
	
	const int packet_length = qFromBigEndian( *(quint16*)(data + IP_PACKET_LENGTH_POS) ); // packet length
	
	// I might read further without going out of the  header but it would be a waste..
	if (src.size() != packet_length)
		return error = QString( "Something went wrong size mismatch: %1 vs. %2" ).arg( packet_length ).arg( src.size() ), false;
	
	m_info.protocol =	(TrProtocol)data[IP_TRANSPORT_PROTOCOL_POS];
	m_info.sourceIP =
		QHostAddress( qFromBigEndian( *(quint32*)(data + IP_SOURCE_ADDRESS_POS) ) );
	m_info.destinationIP =
		QHostAddress( qFromBigEndian( *(quint32*)(data + IP_DESTINATION_ADDRESS_POS) ) );

	const int transport_protocol_start = ipheader_length;

	switch (m_info.protocol) {
		case TCP: { // some tcp stuff
			if (packet_length < TCP_HEADER_MIN_LENGTH + ipheader_length) 
				return error = "Too short for TCP.", false;
			
			// tcp header Length
			const quint8 TCPsize  = (data[transport_protocol_start + TCP_HEADER_LENGTH_POS] & FIRST_HALF) * 4; 
			if (packet_length < (ipheader_length + TCPsize) )
				return error = "Shorter than indicated.", false;

			m_info.sourcePort =
				qFromBigEndian( *(quint16*)(data + transport_protocol_start + TCP_SOURCE_PORT_POS) );
			m_info.destinationPort =
				qFromBigEndian( *(quint16*)(data + transport_protocol_start + TCP_DESTINATION_PORT_POS) );
			//ok size is already checked so I might use this:
			m_last = data[ transport_protocol_start + TCP_FLAGS_POS ] & TCP_FIN_FLAG; // this is fin packet
			m_load = src.right(packet_length - (ipheader_length + TCPsize) );
			return true;
		}
		
		case UDP: {
			if (packet_length < (ipheader_length + sizeof( UDPHeader )))
				return error = "Too short for UDP", false;
			const UDPHeader& udp_header = *(UDPHeader*)(data + ipheader_length);
			m_info.sourcePort = qFromBigEndian( udp_header.source_port );
			m_info.destinationPort = qFromBigEndian( udp_header.destination_port );
			m_load = src.right( packet_length - (ipheader_length + sizeof(UDPHeader)) );
			return true;
		}
		default:
			return error = "Unsupported transport protocol", false;
	}
	return error = "Should never see me", false;
}
