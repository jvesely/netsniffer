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
	const IPv4Header& ip_header = *(IPv4Header*)src.data();
	//const quint8 tmpByte = data[0]; // first byte
//	const quint8 version = data[0] >> 4; // IP version (first 4 bits)
//	Q_ASSERT (version == ip_header.version); 
	
	if (ip_header.version != IPV4)
		return error = QString( "Bad IP version: %1" ).arg( ip_header.version ), false;

	//number of 32bit words in IP header, second half of the first byte
//	const quint8 ipheader_length = (data[0] & (~FIRST_HALF) ) * 4; // to bytes
		const quint8 ipheader_length = ip_header.header_length * 4;
//	Q_ASSERT (ipheader_length == ip_header.header_length * 4);
	
	if (src.size() < (ip_header.header_length * 4))
		return error = "Corrupted: packet < indicated header", false;
	
	//const uint packet_length = qFromBigEndian( *(quint16*)(data + IP_PACKET_LENGTH_POS) ); // packet length
	const uint packet_length = qFromBigEndian( ip_header.total_length );
	//Q_ASSERT (packet_length == qFromBigEndian( ip_header.total_length ));

	// I might read further without going out of the  header but it would be a waste..
	if (src.size() != packet_length)
		return error = QString( "Something went wrong size mismatch: %1 vs. %2" ).arg( packet_length ).arg( src.size() ), false;
	
	m_info.protocol =	(TrProtocol)ip_header.protocol;
	//(TrProtocol)data[IP_TRANSPORT_PROTOCOL_POS];
//	Q_ASSERT (m_info.protocol == ip_header.protocol);
	m_info.sourceIP.setAddress( qFromBigEndian( ip_header.source_address ) );
//		QHostAddress( qFromBigEndian( *(quint32*)(data + IP_SOURCE_ADDRESS_POS) ) );
//	Q_ASSERT (m_info.sourceIP.toIPv4Address() == qFromBigEndian( ip_header.source_address ));

	m_info.destinationIP.setAddress( qFromBigEndian( ip_header.destination_address ) );
//		QHostAddress( qFromBigEndian( *(quint32*)(data + IP_DESTINATION_ADDRESS_POS) ) );
//	Q_ASSERT (m_info.destinationIP.toIPv4Address() == qFromBigEndian( ip_header.destination_address ));
//	const int transport_protocol_start = ipheader_length;
//	qDebug() << src.toHex();

	switch (m_info.protocol) {
		case TCP: { // some tcp stuff
			if (packet_length < sizeof(TCPHeader) + ipheader_length) 
				return error = "Too short for TCP.", false;
//			Q_ASSERT (sizeof(TCPHeader) == 20);
			const TCPHeader& header = *(TCPHeader*)(data + ipheader_length);
				
			// tcp header Length
//			const quint8 TCPsize = ((((quint8)data[transport_protocol_start + TCP_HEADER_LENGTH_POS]) >> 4) * 4);
//			qDebug() << TCPsize << header.header_length << (quint8)data[transport_protocol_start + TCP_HEADER_LENGTH_POS];
//			Q_ASSERT (TCPsize == header.header_length * 4);
			if (packet_length < (ipheader_length + header.header_length * 4) )
				return error = "Shorter than indicated.", false;

			m_info.sourcePort = qFromBigEndian( header.source_port );
			m_info.destinationPort = qFromBigEndian( header.destination_port );
			m_last = header.fin;
			m_load = src.right(packet_length - (ipheader_length + header.header_length) );
//			 Q_ASSERT (header.source_port == *(quint16*)(data + transport_protocol_start + TCP_SOURCE_PORT_POS) );
//       Q_ASSERT (header.destination_port == *(quint16*)(data + transport_protocol_start + TCP_DESTINATION_PORT_POS) );
			//ok size is already checked so I might use this:
//			Q_ASSERT (m_last == (data[ transport_protocol_start + TCP_FLAGS_POS ] & TCP_FIN_FLAG)); // this is fin packet
			return true;
		}
		
		case UDP: {
			if (packet_length < (ipheader_length + sizeof( UDPHeader )))
				return error = "Too short for UDP", false;
			const UDPHeader& header = *(UDPHeader*)(data + ipheader_length);
			m_info.sourcePort = qFromBigEndian( header.source_port );
			m_info.destinationPort = qFromBigEndian( header.destination_port );
			m_load = src.right( packet_length - (ipheader_length + sizeof(UDPHeader)) );
//			Q_ASSERT (header.source_port == *(quint16*)(data + transport_protocol_start + UDP_SOURCE_PORT_POS) );
//			Q_ASSERT (header.destination_port == *(quint16*)(data + transport_protocol_start + UDP_DESTINATION_PORT_POS) );
			return true;
		}
		default:
			return error = "Unsupported transport protocol", false;
	}
	return error = "Should never see me", false;
}
