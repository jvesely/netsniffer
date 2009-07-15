#pragma once

#include "IRecognizer.h"
#include "struct/SafeHash.h"

#define ENGINE_NAME "DNS Recognizer"

class IConnection;
class IDNSCache;

class DnsRecognizer: public IRecognizer
{
public:
	typedef SafeHash<const IConnection*, QVariant> CommentStore;

	DnsRecognizer(): mCache( NULL ) {};
	inline const QString name() const { return ENGINE_NAME; };
	virtual bool guess( const IConnection* connection );
	virtual bool parse( IConnection* connection);
	virtual QVariant comment( IConnection* connection )
		{ return mComments.value( connection, "No comment" ); }
	virtual bool showDetails( IConnection* connection );

	inline void setDnsCache( IDNSCache* cache )
		{ mCache = cache; };
	
private:
	const QString parsePacket( const QByteArray& data ) const;
	const QStringList parseQuestions( uint count, const QByteArray& data, uint& strpos ) const;
	const QStringList parseAnswers( uint count, const QByteArray& data, uint& strpos ) const;
	const QString parseName( const char* data, uint& pos, uint size, uint depth = 0 ) const;

	IDNSCache* mCache;
	CommentStore mComments;

	static const int DEFAULT_DEPTH = 5;
};
