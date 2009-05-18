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
	virtual bool  quickLook( IRecognizer::QuickResult*, IConnection* connection);
	QWidget* analyze( const IConnection * con );

	inline void setDnsCache( IDNSCache* cache )
		{ m_cache = cache; };
	
	const IRecognizer::QuickResult quickLook( const IConnection* connection ) const;

private:
	const QString parseQuick( const QByteArray& data ) const;
	const QStringList parseQuestions( uint count, const QByteArray& data, uint& strpos ) const;
	const QStringList parseAnswers( uint count, const QByteArray& data, uint& strpos ) const;
	const QString parseName( const char* data, uint& pos, uint size, uint depth = 0 ) const;

//	QString parseQuestion( const QByteArray data ) const;
//	QString parseReply( const QByteArray data ) const;

//	QPair<QString, int> parseAnswer( int pos, const QByteArray data ) const;
//	QPointer<QListWidget> forward;
//	QPointer<QListWidget> backward;
	IDNSCache* m_cache;

//	QPair<QString, int> getName( int pos, const QByteArray data, int depth = DEFAULT_DEPTH ) const;
//	int getQEnd( int pos, const QByteArray data ) const;

//	static const int DEFAULT_DEPTH = 5;
};
