#include "NetworkInfo.h"
#include "DnsRecognizer.h"
#include "opcode.h"
#include "errors.h"
#include "dnstypes.h"


#define EMPTY QPair<QString, QString>("", "")

#define DNS 53
#define WINS 137

#define FIRST_BIT  128 //0xFO
#define OPCODE_MASK 112 // 64 + 32 + 16

/*----------------------------------------------------------------------------*/
QPair<QString, QString> DnsRecognizer::quickLook( const IConnection* connection) const
{
	//qDebug() << "Recognizing: " << con;
// check ports
	NetworkInfo info = connection->networkInfo();

	if (	info.sourcePort != DNS && 
				info.sourcePort != WINS && 
				info.destinationPort != DNS && 
				info.destinationPort != WINS
		)
		return EMPTY;
	
	const QByteArray dataForw = connection->getLastPacketForward();
	bool isQuestion;
	QString forw("Empty");
	if (!dataForw.isEmpty()){
		isQuestion = !(dataForw.at(2) & FIRST_BIT); // forward can not be empty
		forw = isQuestion?"DNS Query: ":"Answer: ";
		forw += isQuestion?parseQuestion(dataForw):parseReply(dataForw);
	//	qDebug() << "Forward: " << forw << "\n" << "DUMP:\n" << dataForw.toHex();
	} 

	QString back("Empty");
	const QByteArray dataBack =  connection->getLastPacketBack();
	if (!dataBack.isEmpty())
	{
		isQuestion = !(dataBack.at(2) & FIRST_BIT);
		back = (isQuestion)?"DNS Query ":"Answer: ";
		back +=	isQuestion?parseQuestion(dataBack):parseReply(dataBack);
	}
	qDebug() << "Back(" << back << "):\n" << dataBack.toHex();
	return QPair<QString, QString>(forw, back);
}
/*----------------------------------------------------------------------------*/
QString DnsRecognizer::parseQuestion(const QByteArray data) const{
	// only parse first question
	int opcode = (data.at(2) & OPCODE_MASK); // third byte, 1bit QR 4 bit
	//qDebug() << "Opcode: " << opcode;
	int pos = 12;	//header out
	int qnum = qFromBigEndian(*(quint16*)(data.data() + 4)); // 5th and 6th octet
	//check number of questions present
	QString res;
	if (qnum > 0) 
		res = getName(pos, data).first; // get question
	 else
		res = "No questions";
	

	return getOpCode(opcode) + ": " + res;
}
/*----------------------------------------------------------------------------*/
QString DnsRecognizer::parseReply(const QByteArray data) const{
	int pos = 12; // skip header
	int qnum = qFromBigEndian(*(quint16*)(data.data() + 4)); // 5th and 6th octet
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
QString DnsRecognizer::getOpCode(int code) const {
	switch(code){
		case 0: return OP_CODE_0;
		case 1: return OP_CODE_1;
		case 2: return OP_CODE_2;
		case 4: return OP_CODE_4;
		case 5: return OP_CODE_5;
		default: return OP_CODE_DEF;
	}
	return OP_CODE_DEF;
}
/*----------------------------------------------------------------------------*/
QWidget *  DnsRecognizer::analyze ( const IConnection * con ) {
	qDebug() << "Engine got connection " << con;
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
}
/*----------------------------------------------------------------------------*/
QWidget * DnsRecognizer::getOptionsPage() const{
	return NULL; // no options for me
}

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
		case type_A:  // ipv4 address
			Q_ASSERT(dlen == 4);
			QHostAddress addr(qFromBigEndian(*(quint32*)(data.data() + pos)));
			//qDebug() << "Yeah, found ipadress " << addr;
			//emit dnsRecord(addr, answer);

			answer += " A ";
			answer.append(addr.toString());
			break;


	}
	pos += dlen;
	return QPair<QString, int>(answer, pos);
}
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
