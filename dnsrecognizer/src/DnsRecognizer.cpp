#include "DnsRecognizer.h"
#include <QtPlugin>


#define INFO_TEXT "This plugin recognizes dns comunication."
#define EMPTY_ANSWER QPair<QString, QString>("", "")

Q_EXPORT_PLUGIN2(dnsRecognizer, DnsRecognizer)


QString DnsRecognizer::getInfo() const {
	return INFO_TEXT;
}
/*----------------------------------------------------------------------------*/
QPair<QString, QString> DnsRecognizer::quickLook(
                              QByteArray dataIn,
                              QByteArray dataOut,
                              QHostAddress addrSrc,
                              QHostAddress addrDest,
                              quint16 portSrc,
                              quint16 portDest,
                              int proto
                                    ) const {

	return QPair<QString, QString>("Touched by dnsrecognizer", "Touched by dnsrecognizer");
}
/*----------------------------------------------------------------------------*/
bool DnsRecognizer::recognize (
                              QByteArray dataIn,
                              QByteArray dataOut,
                              QHostAddress addrSrc,
                              QHostAddress addrDest,
                              quint16 portSrc,
                              quint16 portDest,
                              int proto
                                    ) const {
	return true;
}
/*----------------------------------------------------------------------------*/
QWidget * DnsRecognizer::getOptionsPage() const{
	return NULL;
}
