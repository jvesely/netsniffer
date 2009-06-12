#pragma once

#include "IRecognizer.h"

#define ENGINE_NAME "DNS Recognizer"

class IConnection;
class IDNSCache;

class DnsRecognizer: public IRecognizer
{
public:
	DnsRecognizer():m_cache( NULL ){};
	inline const QString name() const { return ENGINE_NAME; };
	virtual bool  parse( QStringList* comments, IConnection* connection);
	QWidget* analyze( const IConnection* con );

	inline void setDnsCache( IDNSCache* cache )
		{ m_cache = cache; };
	
//	const bool parse( const IConnection* connection ) const;

private:
	const QString parsePacket( const QByteArray& data ) const;
	const QStringList parseQuestions( uint count, const QByteArray& data, uint& strpos ) const;
	const QStringList parseAnswers( uint count, const QByteArray& data, uint& strpos ) const;
	const QString parseName( const char* data, uint& pos, uint size, uint depth = 0 ) const;

	IDNSCache* m_cache;

	static const int DEFAULT_DEPTH = 5;
};
