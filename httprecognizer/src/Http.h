#pragma once

#include "struct/SafeHash.h"

namespace Http
{
	typedef QPair<QHttpRequestHeader, QByteArray> Request;
	typedef QPair<QHttpResponseHeader, QSharedPointer<QByteArray> > Response;
	
	typedef QList<QPair<Request, QHttpResponseHeader> > Session;
	typedef QCache<QUrl, Response> Cache;
};
/*---------------------------------------------------------------------------*/
inline uint qHash( const QHttpRequestHeader& header )
{
	return qHash( header.toString() );
}
/*---------------------------------------------------------------------------*/
inline uint qHash( const QUrl& url )
{
	return qHash( url.toString() );
}
