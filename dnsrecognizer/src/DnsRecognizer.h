#pragma once

#include "IRecognizer.h"

#define DEFAULT_DEPTH 5 //default name resovling depth
#define ENGINE_NAME "DNS Recognizer"

class IConnection;
class IDNSCache;

class DnsRecognizer: public IRecognizer
{
public:
	inline const QString name() const { return ENGINE_NAME; };
	const IRecognizer::QuickResult quickLook( const IConnection* connection ) const;
	QWidget* analyze( const IConnection * con );

	inline void setDnsCache( IDNSCache* cache )
		{ m_cache = cache; };

private:
	QString parseQuestion( const QByteArray data ) const;
	QString parseReply( const QByteArray data ) const;
	QPair<QString, int> parseAnswer( int pos, const QByteArray data ) const;
//	QPointer<IConnection>  conn;
//	QPointer<QListWidget> forward;
//	QPointer<QListWidget> backward;
	IDNSCache* m_cache;

	inline QString getOpCode( int code ) const;
	inline QPair<QString, int> getName( int pos, const QByteArray data, int depth = DEFAULT_DEPTH ) const;
	inline int getQEnd( int pos, const QByteArray data ) const;

};
