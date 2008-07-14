#include "ARecognizerEngine.h"



class DnsRecognizer:public ARecognizerEngine {

	Q_OBJECT
	Q_INTERFACES(ARecognizerEngine)

public:
	~DnsRecognizer(){};
	QString getInfo() const;
public slots:
	QPair<QString, QString> quickLook(
															QByteArray dataIn, 
															QByteArray dataOut, 
															QHostAddress addrSrc, 
															QHostAddress addrDest, 
															quint16 portSrc, 
															quint16 portDest, 
															int proto
																		) const;
	bool recognize (
					QByteArray dataIn,
					QByteArray dataOut,
					QHostAddress addrSrc,
					QHostAddress addrDest,
					quint16 portSrc,
					quint16 portDest,
					int proto
							) const;
	QWidget * getOptionsPage() const;	
};
