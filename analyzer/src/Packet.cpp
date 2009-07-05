#include "Packet.h"
#include "ProtocolHeaders.h"

#define DEBUG_TEXT "[ Packet ]: "
#include "debug.h"

static const int IPV4 = 4; // ip identifier for IPv4

/*----------------------------------------------------------------------------*/
Packet Packet::parse( const QByteArray& src )
{
	if (src.size() < (int)sizeof(Header::IPv4))
		 throw std::runtime_error( "Corrupted: Too short for IP" );
	
	const Header::IPv4& ip_header = *(Header::IPv4*)src.data();
	
	if (ip_header.version != IPV4)
		throw std::runtime_error( QString( "Bad IP version: %1" ).arg( ip_header.version ).toStdString() );

	//number of 32bit words in IP header convert to bytes
	const quint8 ipheader_length = ip_header.header_length * 4;
	
	if (src.size() < (ip_header.header_length * 4))
		throw std::runtime_error( "Corrupted: Packet too small for IP header." );
	
	const int packet_length = qFromBigEndian( ip_header.total_length );

	// I might read further without going out of the header but it would be a waste..
	if (src.size() != packet_length)
		throw std::runtime_error(
			QString( "Corrupted: packet size does not match %1 vs. %2." ).arg( packet_length ).arg( src.size() ).toStdString()
			);

	NetworkInfo info;
	
	info.protocol =	(TrProtocol)ip_header.protocol;
	info.sourceIP.setAddress( qFromBigEndian( ip_header.source_address ) );
	info.destinationIP.setAddress( qFromBigEndian( ip_header.destination_address ) );

	switch (info.protocol)
	{
		case TCP:  // some tcp stuff
		{
			if (packet_length < (int)sizeof(Header::TCP) + ipheader_length) 
				throw std::runtime_error( "Corrupted: packet too short to hold TCP header" );

			const Header::TCP& transport_header = *(Header::TCP*)(src.data() + ipheader_length);
			if ( packet_length < (ipheader_length + transport_header.header_length * 4) )
				throw std::runtime_error( "Corrupted: Packet can't hold full TCP header" );

			info.sourcePort = qFromBigEndian( transport_header.source_port );
			info.destinationPort = qFromBigEndian( transport_header.destination_port );

			const QByteArray load =
				src.mid( (ip_header.header_length + transport_header.header_length) * 4 );
			return Packet( info, load, transport_header.fin );
		}
		
		case UDP: {
			if (packet_length < (ipheader_length + (int)sizeof(Header::UDP)))
				throw std::runtime_error( "Corrupted: Too short to hold UDP packet" );

			const Header::UDP& transport_header = *(Header::UDP*)(src.data() + ipheader_length);
			info.sourcePort = qFromBigEndian( transport_header.source_port );
			info.destinationPort = qFromBigEndian( transport_header.destination_port );
			const QByteArray load =
				src.mid( ipheader_length + sizeof( Header::UDP ) );
			return Packet( info, load );
		}
		default:
			throw std::runtime_error( "Corrupted: Unsupported protocol type." );
	}
}
