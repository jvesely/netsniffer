#include "Packet.h"
#include "headers.h"

#define DEBUG_TEXT "[ Packet ]: "
#include "debug.h"

static const int IPV4 = 4; // ip identifier for IPv4

/*----------------------------------------------------------------------------*/
Packet::Packet( const QByteArray& src ): m_last( true )
{
	QString error;
	if (!parse( src, error ))
		throw std::runtime_error( error.toStdString() );
}
/*----------------------------------------------------------------------------*/
bool Packet::parse( const QByteArray& src, QString& error ) throw()
{
	if (src.size() < sizeof(IPv4Header))
		return error = "Too short for IP", false;
	
	const char* data = src.data();
	const IPv4Header& ip_header = *(IPv4Header*)src.data();
	
	if (ip_header.version != IPV4)
		return error = QString( "Bad IP version: %1" ).arg( ip_header.version ), false;

	//number of 32bit words in IP header
	const quint8 ipheader_length = ip_header.header_length * 4;
	
	if (src.size() < (ip_header.header_length * 4))
		return error = "Corrupted: packet < indicated header", false;
	
	const uint packet_length = qFromBigEndian( ip_header.total_length );

	// I might read further without going out of the  header but it would be a waste..
	if (src.size() != packet_length)
		return error = QString( "Something went wrong size mismatch: %1 vs. %2" ).arg( packet_length ).arg( src.size() ), false;
	
	m_info.protocol =	(TrProtocol)ip_header.protocol;
	m_info.sourceIP.setAddress( qFromBigEndian( ip_header.source_address ) );
	m_info.destinationIP.setAddress( qFromBigEndian( ip_header.destination_address ) );

	switch (m_info.protocol) {
		case TCP: { // some tcp stuff
			if (packet_length < sizeof(TCPHeader) + ipheader_length) 
				return error = "Too short for TCP.", false;
			const TCPHeader& header = *(TCPHeader*)(data + ipheader_length);
				
			if (packet_length < (ipheader_length + header.header_length * 4) )
				return error = "Shorter than indicated.", false;

			m_info.sourcePort = qFromBigEndian( header.source_port );
			m_info.destinationPort = qFromBigEndian( header.destination_port );
			m_last = header.fin;
			m_load = src.right(packet_length - (ipheader_length + header.header_length * 4) );
			return true;
		}
		
		case UDP: {
			if (packet_length < (ipheader_length + sizeof( UDPHeader )))
				return error = "Too short for UDP", false;
			const UDPHeader& header = *(UDPHeader*)(data + ipheader_length);
			m_info.sourcePort = qFromBigEndian( header.source_port );
			m_info.destinationPort = qFromBigEndian( header.destination_port );
			m_load = src.right( packet_length - (ipheader_length + sizeof(UDPHeader)) );
			return true;
		}
		default:
			return error = "Unsupported transport protocol", false;
	}
	return error = "Should never see me", false;
}
