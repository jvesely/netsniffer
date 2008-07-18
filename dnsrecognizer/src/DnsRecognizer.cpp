#include "DnsRecognizer.h"
#include "opcode.h"
#include <QtPlugin>
#include <QtCore>


#define INFO_TEXT "This plugin recognizes dns comunication."
#define EMPTY QPair<QString, QString>("", "")

#define DNS 53
#define WINS 137

#define FIRST_BIT  128 //0xFO
#define OPCODE_MASK 112 // 64 + 32 + 16

Q_EXPORT_PLUGIN2(dnsRecognizer, DnsRecognizer)


QString DnsRecognizer::getInfo() const {
	return INFO_TEXT;
}
/*----------------------------------------------------------------------------*/
QPair<QString, QString> DnsRecognizer::quickLook(
                              QByteArray& dataForw,
                              QByteArray& dataBack,
                              QHostAddress addrSrc,
                              QHostAddress addrDest,
                              quint16 portSrc,
                              quint16 portDest,
                              int proto
                                    ) const {
	if (portSrc != DNS && portSrc != DNS && portDest != DNS && portDest != DNS)
		return EMPTY;
	// ok it uses right ports, it could be DNS or WINS
	//if (proto != UDP)
	//	return EMPTY;
	bool isQuestion = !(dataForw.at(2) & FIRST_BIT); // forward can not be empty
	QString forw = isQuestion?" DNS Query: ":" Answer: ";
	forw += isQuestion?parseQuestion(dataForw):parseAnswer(dataForw);
	qDebug() << "Forward: " << forw << "\n" << "DUMP:\n" << dataForw.toHex();

	QString back("Empty");
	if (!dataBack.isEmpty()){
		isQuestion = !(dataBack.at(2) & FIRST_BIT);
		back = (isQuestion)?" DNS Query ":" Answer: ";
		back +=	isQuestion?parseQuestion(dataBack):parseAnswer(dataBack);
	}
	qDebug() << "Back(" << back << "):\n" << dataBack.toHex();
	return QPair<QString, QString>(forw, back);
}
/*----------------------------------------------------------------------------*/
QString DnsRecognizer::parseQuestion(const QByteArray data) const{
	// only parse first question
	int opcode = (data.at(2) & OPCODE_MASK); // third byte, 1bit QR 4 bit
	qDebug() << "Opcode: " << opcode;
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
QString DnsRecognizer::parseAnswer(const QByteArray data) const{
	int pos = 12; // skip header
	int qnum = qFromBigEndian(*(quint16*)(data.data() + 4)); // 5th and 6th octet
	int anum = qFromBigEndian(*(quint16*)(data.data() + 6)); // 5th and 6th octet
	qDebug () << "Number of questions: " << qnum << "\nNumber of Answers: "<<anum;
	if (anum == 0 )
		return "No Answers";
	// Questions --  no way to avoid them
	for (int i = 0; i < qnum; ++i) {
		pos = getName(pos, Data).second;
		pos += 5; // 1 byte type and 1 byte class, + begining of next;
	}
	for (int i = 0; i < anum; ++i) {
	}
	
	return ("");
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
bool DnsRecognizer::recognize (
                              QByteArray dataForw,
                              QByteArray dataBack,
                              QHostAddress addrSrc,
                              QHostAddress addrDest,
                              quint16 portSrc,
                              quint16 portDest,
                              int proto
                                    ) const {
	Q_UNUSED(dataForw);
	Q_UNUSED(dataBack);
	return true;
}
/*----------------------------------------------------------------------------*/
QWidget * DnsRecognizer::getOptionsPage() const{
	return NULL; // no options for me
}

QPair<QString, int> DnsRecognizer::getName(int pos, const QByteArray data) const{
	int len = data.length();
	QString ret;
	for(int count = data[pos];count && (pos + count + 1 < len);)
	{
		++pos;
		if (count > 128)
			return QPair<QString,int>(ret + getName(data[pos], data).first, pos);
		if (len < pos + count ) // there is desired length + end point/next legnth + more because of type 
			return QPair<QString, int>("Invalid query", 0);
	
		ret += QString::fromAscii(data.mid(pos, count)) + ".";
		pos += count;
		count = data[pos];
	}
	return QPair<QString,int>(ret, pos);
}
