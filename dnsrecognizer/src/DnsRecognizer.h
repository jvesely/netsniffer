#include "ARecognizerEngine.h"

#define DEFAULT_DEPTH 5 //default name resovling depth


class DnsRecognizer:public ARecognizerEngine {

	Q_OBJECT
	Q_INTERFACES(ARecognizerEngine)

	QString parseQuestion(const QByteArray data) const;
	QString parseReply(const QByteArray data) const;
	QPair<QString, int> parseAnswer(int pos, const QByteArray data) const;

	inline QString getOpCode(int code) const;
	inline QPair<QString, int> getName(int pos, const QByteArray data, int depth = DEFAULT_DEPTH) const;
	inline int getQEnd(int pos, const QByteArray data) const;

public:
	~DnsRecognizer(){};
	QString getInfo() const;
public slots:
	QPair<QString, QString> quickLook( const IConnection * con ) const;
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
