#pragma once

#include "struct/SafeHash.h"

namespace Http
{
	typedef QPair<QHttpRequestHeader, QByteArray> Request;
	typedef QPair<QHttpResponseHeader, QByteArray> Response;
	typedef SafeHash<Request, Response> Session;
};
