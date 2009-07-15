#pragma once

#include "struct/SafeHash.h"

namespace Http
{
	typedef QPair<QHttpRequestHeader, QByteArray> Request;
	typedef QPair<QHttpResponseHeader, QSharedPointer<QByteArray> > Response;
	
	typedef QList<QPair<Request, QHttpResponseHeader> > Session;
	typedef QCache<Request, Response> Cache;
};
