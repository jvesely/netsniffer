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
	virtual bool  quickLook( IRecognizer::QuickResult*, const IConnection* connection) const;
	QWidget* analyze( const IConnection * con );

	inline void setDnsCache( IDNSCache* cache )
		{ m_cache = cache; };

private:
	QString parseQuick( const QByteArray& data ) const;
	QString parseQuestion( const QByteArray data ) const;
	QString parseReply( const QByteArray data ) const;
	const QStringList parseQuestions( uint count, const QByteArray& data, uint& strpos ) const;

	QString parseName( const char* data, uint& pos, uint size ) const;

	QPair<QString, int> parseAnswer( int pos, const QByteArray data ) const;
//	QPointer<QListWidget> forward;
//	QPointer<QListWidget> backward;
	IDNSCache* m_cache;

	//const QString getOpCode( uint code ) const;
	QPair<QString, int> getName( int pos, const QByteArray data, int depth = DEFAULT_DEPTH ) const;
	int getQEnd( int pos, const QByteArray data ) const;

};
