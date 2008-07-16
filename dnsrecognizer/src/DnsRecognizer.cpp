#include "DnsRecognizer.h"
#include <QtPlugin>


#define INFO_TEXT "This plugin recognizes dns comunication."
#define EMPTY QPair<QString, QString>("", "")

#define DNS 53
#define WINS 137

#define FIRST_BIT  128 //0xFO

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
	QString back = dataBack.isEmpty()?"Empty":(dataBack.at(2) & FIRST_BIT)?"Answer":"Query";
	forw += 	(isQuestion?parseQuestion(dataForw):"");
	qDebug() << "Forward: " << forw;
	
	qDebug() << "Back(" << back << "):\n" << dataBack.toHex();
	//dataForw.clear();
	//dataBack.clear();
	return QPair<QString, QString>(forw, back);
}
/*----------------------------------------------------------------------------*/
QString DnsRecognizer::parseQuestion(QByteArray data) const{
	// only parses first question
	int opcode = (data.at(2) % 128 ) / 8;
	qDebug() << "Opcode: " << opcode;
	data.remove(0, 12); //header out
	QString res;
	for(int count = data.at(0);count && !data.isEmpty() ;){
		data.remove(0, 1);
		if (data.length() < count + 1)
			return ("Invalid query");
		res += data.left(count) + ".";
		data.remove(0, count);
		count = data.at(0);
	}

	return res;
}
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
	return NULL;
}
