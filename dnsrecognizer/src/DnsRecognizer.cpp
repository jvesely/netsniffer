#include "DnsRecognizer.h"
#include "NetworkInfo.h"
#include "IConnection.h"
#include "IDNSCache.h"
#include "errors.h"
#include "Dns.h"


#define EMPTY QPair<QString, QString>("", "")

static const int DNS_PORT = 53;
static const int WINS_PORT = 137;

#define FIRST_BIT  128 //0xFO
#define OPCODE_MASK 112 // 64 + 32 + 16

/*----------------------------------------------------------------------------*/
bool DnsRecognizer::quickLook( IRecognizer::QuickResult* comment, const IConnection* connection ) const
{
	Q_ASSERT (connection);
	Q_ASSERT (sizeof( Dns::Header ) == 12 );
	
	const NetworkInfo& info = connection->networkInfo();
	
	if (	info.sourcePort != DNS_PORT && 
				info.sourcePort != WINS_PORT && 
				info.destinationPort != DNS_PORT && 
				info.destinationPort != WINS_PORT
		)
		return false;
	
	Q_ASSERT (comment);
	{
		const QByteArray data = connection->getLastPacketForward();
		if (!data.isEmpty())
			comment->first = parseQuick( data );
	}
	{
		const QByteArray data = connection->getLastPacketForward();
		if (!data.isEmpty())
			comment->second = parseQuick( data );
	}
	return true;
}
/*----------------------------------------------------------------------------*/
QString DnsRecognizer::parseQuick( const QByteArray& data ) const
{
	const Dns::Header& header = *(Dns::Header*)data.data();
	QString result("Header:  ID:%1\nQR:%2\nOpcode:%3\nRcode:%4\nQd-count:%5\nAn-count:%6\nNS-count:%7\nAR-count:%8\n");
	uint startpos = sizeof( Dns::Header );
	result = result.arg( qFromBigEndian( header.ID ) ).
		arg( header.QR ).arg( header.Opcode ).arg( header.RCODE ).
		arg( qFromBigEndian( header.QDCOUNT ) ).
		arg( qFromBigEndian( header.ANCOUNT ) ).
		arg( qFromBigEndian( header.NSCOUNT ) ).
		arg( qFromBigEndian( header.ARCOUNT ) )
		+ parseQuestions( qFromBigEndian( header.QDCOUNT ), data, startpos ).join("\n")
		+ "\n"  + data.toHex();
	

	return result;
}
/*----------------------------------------------------------------------------*/
const QStringList DnsRecognizer::parseQuestions( uint count, const QByteArray& data, uint& pos ) const
{
	const char* packet = data.data();
	const uint size = data.size();
	QStringList result;
	for (; count; --count) {
		qDebug() << "Parsing:" << data.toHex();
		const QString name = parseName( packet, pos, size );
		const Dns::QuestionData& data = *(Dns::QuestionData*)(packet + pos);
		result.append( name + " " + Dns::typeToString( (Dns::Type)qFromBigEndian( data.QTYPE  ) ) );

		pos += sizeof(Dns::QuestionData);				
	}
	qDebug() << result;
	return result;
}
/*----------------------------------------------------------------------------*/
QString DnsRecognizer::parseName( const char* data, uint& pos, uint size ) const
{
	QString result;
	qDebug() << "Getting name from position " << pos << "from data of size" << size;
	if (pos >= size)
		return "Malformed";
	
	while( data[pos] ) {
//		qDebug() << "on pos" << pos << "is" << data[pos] << (quint8)data[pos];
		//test recursion;
		if ((quint8)data[pos] > 128) {
//			qDebug() << "Position higher than 128" << (quint8)data[pos];
			pos = (quint8)data[pos];
			continue;
		}
		if (1 + pos + (quint8)data[pos] >= size)
			return "Malformed";

		result += QString::fromAscii( (data + pos + 1), data[pos] );
//		qDebug() << "Added: " << QString::fromAscii( (data + pos + 1), data[pos] ) << "length:" << (quint8)data[pos];
		pos += 1 + (quint8)data[pos];
	}
	++pos;
	return result;
}
/*----------------------------------------------------------------------------*/
QString DnsRecognizer::parseQuestion(const QByteArray data) const
{
	// only parse first question
	const Dns::Header& header = *(Dns::Header*)data.data();
	int opcode = (data.at(2) & OPCODE_MASK); // third byte, 1bit QR 4 bit
	Q_ASSERT(opcode == header.Opcode);
	//qDebug() << "Opcode: " << opcode;
	const int pos = 12;	//header out
	const int qnum = qFromBigEndian(*(quint16*)(data.data() + 4)); // 5th and 6th octet
	Q_ASSERT(qnum == qFromBigEndian( header.QDCOUNT ));

	//check number of questions present
	QString res;
	if (qnum > 0) 
		res = getName(pos, data).first; // get question
 	else
		res = "No questions";
	

	return Dns::opcodeToString( opcode ) + ": " + res;
}
/*----------------------------------------------------------------------------*/
QString DnsRecognizer::parseReply(const QByteArray data) const
{
	const Dns::Header& header = *(Dns::Header*)data.data();
	int pos = 12; // skip header
	int qnum = qFromBigEndian(*(quint16*)(data.data() + 4)); // 5th and 6th octet
	Q_ASSERT( qnum == qFromBigEndian( header.QDCOUNT ));
	int anum = qFromBigEndian(*(quint16*)(data.data() + 6)); // 5th and 6th octet
	//qDebug () << "Number of questions: " << qnum << "\nNumber of Answers: "<<anum;
	if (anum == 0 )
		return "No Answers";
	// Questions --  no way to avoid them
	for (int i = 1; i <= qnum; ++i) {
		//qDebug() << "Skipping question " << i << "From pos: "<<pos;
		if(!(pos = getQEnd(pos, data)))
			return ERR_CORRUPTED_QUERY;
		//qDebug() << "to pos "<< pos;
	}
	QString report;
	for (int i = 0; i < anum; ++i) {
		QPair<QString, int> answ = parseAnswer(pos, data);
		report = answ.first;
		pos = answ.second;
	}
	
	return report; 
}
/*----------------------------------------------------------------------------*/
/*
const QString DnsRecognizer::getOpCode(uint code) const
{
	if (code > 5)
		code = 5;
	return Dns::OperationCodes[code];
}
*/
/*----------------------------------------------------------------------------*/
QWidget *  DnsRecognizer::analyze ( const IConnection * con ) {

	qDebug() << "Engine got connection " << con;
	/*
	conn = (IConnection *)con;
	QWidget * results = new QWidget();
	QVBoxLayout * mainLayout = new QVBoxLayout(results);
//	results->setLayout(mainLayout);
	QLabel * nameLabel = new QLabel(name(), results);
	QHBoxLayout * lists = new QHBoxLayout();
	forward = new QListWidget();
	lists->addWidget(forward);
	backward = new QListWidget();
	lists->addWidget(backward);
	mainLayout->addLayout(lists);
//	QHBoxLayout * new

	return results;
	*/
	return NULL;
}
/*----------------------------------------------------------------------------*/
QPair<QString, int> DnsRecognizer::getName(int pos, const QByteArray data, int depth) const{
	//qDebug() << "getting name from pos: " << pos << "depth: " << depth;  

	if (depth == 0)
		return QPair<QString, int>(ERR_CORRUPTED_QUERY, 0);
	int len = data.length();
	QString ret;
	for(quint8 count = data[pos];count;)
	{		
		++pos;
		if (count > 128){
			//qDebug() << "recursion from pos: " << pos << "to pos: " << (quint8)data[pos];
			return QPair<QString,int>(ret + getName(data[pos], data, --depth).first, pos);
		}
		if (len < pos + count ) // there is desired length + end point/next legnth + more because of type 
			return QPair<QString, int>(ERR_CORRUPTED_QUERY, 0);
	
		ret += QString::fromAscii(data.mid(pos, count)) + ".";
		pos += count;
		count = data[pos];
	}
	return QPair<QString,int>(ret, pos);
}
/*----------------------------------------------------------------------------*/
QPair<QString, int> DnsRecognizer::parseAnswer(int pos, const QByteArray data) const {
	//qDebug() << "Parsing answer: " << data.toHex() << "\nfrom position: " << pos;
	QPair<QString, int> res = getName(pos, data);
	int end = res.second;
	//qDebug() << res;
	QString answer;
	if (! end)
		return QPair<QString, int>(ERR_CORRUPTED_QUERY, 0);
	answer += res.first;


	pos = res.second; // now i have the name and can safely parse the rest
	++pos; // get over it
	quint16 type  = qFromBigEndian(*(quint16*)(data.data() + pos ));
	pos += 8; // skip type(2B), skip class (2B) and ttl (4b)
	quint16 dlen = qFromBigEndian(*(quint16*)(data.data() + pos));
	pos += 2;// over dlen
	//qDebug() << "Answer: " << answer << " type:" << type << " datalen: " << dlen;
	
	
	switch (type) {
		case Dns::A:  // ipv4 address
			Q_ASSERT(dlen == 4);
			QHostAddress addr(qFromBigEndian(*(quint32*)(data.data() + pos)));
			//qDebug() << "Yeah, found ipadress " << addr;
			//emit dnsRecord(addr, answer);
			if (m_cache)
				m_cache->insert( addr, answer );
			answer += " A ";
			answer.append(addr.toString());
			break;
	}
	pos += dlen;
	return QPair<QString, int>(answer, pos);
}
/*----------------------------------------------------------------------------*/
int DnsRecognizer::getQEnd(int pos, const QByteArray data) const {
	int len = data.length();
  for(quint8 count = data[pos];count && (pos + count + 1 < len);)
  {
    ++pos;
    if (count > 128)
      return pos + 5;
    if (len < pos + count ) // there is desired length + end point/next legnth + more because of type 
      return 0;

    pos += count;
    count = data[pos];
  }
	return pos + 5; // at the end

}
