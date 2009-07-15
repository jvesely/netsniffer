/*!
 * @file
 * @brief Short description.
 *
 * Long description. I would paste some Loren Ipsum rubbish here, but I'm afraid
 * It would stay that way. Not that this comment is by any means ingenious but
 * at least people can understand it.
 */
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
