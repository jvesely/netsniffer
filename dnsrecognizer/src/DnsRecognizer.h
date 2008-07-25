#pragma once

#include "ARecognizerEngine.h"

#define DEFAULT_DEPTH 5 //default name resovling depth
#define ENGINE_NAME "DNS Recognizer"
#define ENGINE_INFO "Parses DNS queries and replys, emits signal when type A reply parsed"


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
	inline QString name() const { return ENGINE_NAME; };
	inline QString info() const { return ENGINE_INFO; };
public slots:
	QPair<QString, QString> quickLook( const IConnection * con ) const;
	QWidget * analyze ( const IConnection * con );
	QWidget * getOptionsPage() const;	
};
