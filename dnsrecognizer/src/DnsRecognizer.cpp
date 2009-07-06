#include "DnsRecognizer.h"
#include "NetworkInfo.h"
#include "IConnection.h"
#include "IDNSCache.h"
#include "errors.h"
#include "Dns.h"

#define DEBUG_TEXT "[ DnsRecognizer ]:"
#include "debug.h"

static const int DNS_PORT = 53;
static const int WINS_PORT = 137;

/*----------------------------------------------------------------------------*/
bool DnsRecognizer::guess( const IConnection* connection )
{
	Q_ASSERT (connection);
	const NetworkInfo& info = connection->networkInfo();
	
	if (	info.sourcePort != DNS_PORT && info.destinationPort != DNS_PORT && 
				info.sourcePort != WINS_PORT &&	info.destinationPort != WINS_PORT	)
		return false;

	return true;
}
/*----------------------------------------------------------------------------*/
bool DnsRecognizer::parse( IConnection* connection )
{
	Q_ASSERT (connection);
	Q_ASSERT (sizeof( Dns::Header ) == 12 );
	
	const IConnection::DirectedPacket packet = connection->nextPacket();
	if (!packet.second.isEmpty()) {
		m_comments[connection] = parsePacket( packet.second );
	}
	return true;
}
/*----------------------------------------------------------------------------*/
bool  DnsRecognizer::showDetails( IConnection* connection )
{
	Q_UNUSED (connection);
	PRINT_DEBUG ("Engine got connection " << connection);
	QMessageBox::information( NULL, name(), QString( "%1 does not provide any connection details, for the list of captured DNS names see DNS Cache tab in options." ).arg( name() ), QMessageBox::Ok );
	return false;
}
/*----------------------------------------------------------------------------*/
const QString DnsRecognizer::parsePacket( const QByteArray& data ) const
{
	const Dns::Header& header = *(Dns::Header*)data.data();
	QString result("Header:  ID:%1 QR:%2 OP-code:%3 R-code:%4 QD-count:%5 AN-count:%6 NS-count:%7 AR-count:%8\n");
	
	uint startpos = sizeof( Dns::Header );
	
	result = result.arg( qFromBigEndian( header.ID ) ).
		arg( header.QR ).
		arg( Dns::opcodeToString( header.Opcode ) ).
		arg( header.RCODE ).
		arg( qFromBigEndian( header.QDCOUNT ) ).
		arg( qFromBigEndian( header.ANCOUNT ) ).
		arg( qFromBigEndian( header.NSCOUNT ) ).
		arg( qFromBigEndian( header.ARCOUNT ) );
	const QStringList questions = parseQuestions( qFromBigEndian( header.QDCOUNT ), data, startpos );
	const QStringList answers = parseAnswers( qFromBigEndian( header.ANCOUNT ), data, startpos );

	result += (	questions + answers	).join( "\n" );
	
	return result;
}
/*----------------------------------------------------------------------------*/
const QStringList DnsRecognizer::parseAnswers( uint count, const QByteArray& data, uint& pos ) const
{
	const char* packet = data.data();
	const uint size = data.size();
	QStringList result;
	for (; count; --count) {
		PRINT_DEBUG ("Parsing answer" << data.toHex() << "on pos:" << pos << "size" << size);
		const QString name = parseName( packet, pos, size );
		PRINT_DEBUG ("Answer name ended on position:" << pos);
		const Dns::AnswerData& data = *(Dns::AnswerData*)(packet + pos);
		PRINT_DEBUG ("Answerdata:" << sizeof(Dns::AnswerData));

		
		pos += sizeof(Dns::AnswerData);				
		
		switch ( qFromBigEndian( data.QTYPE  ) )
		{
			case Dns::A:
			{
				Q_ASSERT (qFromBigEndian( data.DATA_LENGTH ) == sizeof(quint32) );
				const QHostAddress address( qFromBigEndian( *(quint32*)(packet + pos) ) );
				if (m_cache) {
					m_cache->insert( address, name );
				}
				
				result.append( name +
					" " + Dns::typeToString( (Dns::Type)qFromBigEndian( data.QTYPE  ) ) +
					" " + Dns::classToString( qFromBigEndian( data.QCLASS ) ) +
					" " + address.toString() );
				break;
			} 
			case Dns::PTR:
			{
				const QHostAddress address( 
#if Q_BYTE_ORDER == Q_LITTLE_ENDIAN
					qFromBigEndian( QHostAddress( name.section( '.', 0, 3 ) ).toIPv4Address() )
#else
					qFromLittleEndian( QHostAddress( name.section( '.', 0, 3 ) ).toIPv4Address() )
#endif
						);
				
				uint temp_pos = pos;
				const QString ptr_name = parseName( packet, temp_pos, size );

				if (m_cache) {
					m_cache->insert( address, ptr_name );
				}
				
				result.append( name +
					" " + Dns::typeToString( (Dns::Type)qFromBigEndian( data.QTYPE  ) ) +
					" " + Dns::classToString( qFromBigEndian( data.QCLASS ) ) +
					" " + ptr_name );
			}
			default:
				result.append( name +
					" " + Dns::typeToString( (Dns::Type)qFromBigEndian( data.QTYPE  ) ) +
					" " + Dns::classToString( qFromBigEndian( data.QCLASS ) ) );
		}

		pos += qFromBigEndian( data.DATA_LENGTH );
	}
	return result;
}
/*----------------------------------------------------------------------------*/
const QStringList DnsRecognizer::parseQuestions( uint count, const QByteArray& data, uint& pos ) const
{
	const char* packet = data.data();
	const uint size = data.size();
	QStringList result;
	for (; count; --count) {
		PRINT_DEBUG ("Parsing question on pos" << pos);
		const QString name = parseName( packet, pos, size );
		const Dns::QuestionData& data = *(Dns::QuestionData*)(packet + pos);
		
		result.append( name +
			" " + Dns::typeToString( (Dns::Type)qFromBigEndian( data.QTYPE  ) ) +
			" " + Dns::classToString( qFromBigEndian( data.QCLASS ) ) );

		pos += sizeof(Dns::QuestionData);
		PRINT_DEBUG ("question ends on pos" << pos);
	}
	PRINT_DEBUG (result);
	return result;
}
/*----------------------------------------------------------------------------*/
const QString DnsRecognizer::parseName( const char* data, uint& pos, uint size, uint depth ) const
{
	if (pos >= size)
		return "Malformed";
	
	QStringList result;
	
	while( data[pos] )
	{
		if ((quint8)data[pos] >= 0xc0) /* First 2 bits set mean it's pointer */
		{
			/* remove first two bits */
			uint new_pos = qFromBigEndian(*(quint16*)(data+pos)) & ~0xc000;

			pos += 2;
			result << parseName( data, new_pos, size, depth ); 
			return result.join(".");
		}
		
		if (1 + pos + (quint8)data[pos] >= size)
			return "Malformed";

		result << QString::fromAscii( (data + pos + 1), data[pos] );
		pos += 1 + (quint8)data[pos];
	}
	++pos;
	return result.join(".");
}
/*----------------------------------------------------------------------------*/
