#pragma once

#include "IRecognizer.h"
#include "IConnection.h"

#define DEFAULT_DEPTH 5 //default name resovling depth
#define ENGINE_NAME "DNS Recognizer"
#define ENGINE_INFO "Parses DNS queries and replies, emits signal when type A reply parsed"


class DnsRecognizer:public IRecognizer
{
public:
	~DnsRecognizer(){};
	inline const QString name() const { return ENGINE_NAME; };
	inline const QString info() const { return ENGINE_INFO; };

	QPair<QString, QString> quickLook( const IConnection * con ) const;
	QWidget* analyze ( const IConnection * con );
	QWidget* getOptionsPage() const;	

private:
	Q_OBJECT;

	QString parseQuestion( const QByteArray data ) const;
	QString parseReply( const QByteArray data ) const;
	QPair<QString, int> parseAnswer( int pos, const QByteArray data ) const;
	QPointer<IConnection>  conn;
	QPointer<QListWidget> forward;
	QPointer<QListWidget> backward;

	inline QString getOpCode( int code ) const;
	inline QPair<QString, int> getName( int pos, const QByteArray data, int depth = DEFAULT_DEPTH ) const;
	inline int getQEnd( int pos, const QByteArray data ) const;

};
