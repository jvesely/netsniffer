#include "ARecognizerEngine.h"



class DnsRecognizer:public ARecognizerEngine {

	Q_OBJECT
	Q_INTERFACES(ARecognizerEngine)

	QString parseQuestion(const QByteArray data) const;
	QString parseAnswer(const QByteArray data) const;
	inline QString getOpCode(int code) const;
	inline QPair<QString, int> getName(int pos, const QByteArray data) const;

public:
	~DnsRecognizer(){};
	QString getInfo() const;
public slots:
	QPair<QString, QString> quickLook(
															QByteArray& dataForw, 
															QByteArray& dataBack, 
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
