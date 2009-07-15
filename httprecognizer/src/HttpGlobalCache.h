#pragma once

#include "Http.h"
#include "Singleton.h"

class HttpGlobalCache: public Singleton<HttpGlobalCache>, public Http::Cache
{
	HttpGlobalCache(){};
	friend class Singleton<HttpGlobalCache>;
};

#define HTTP_CACHE HttpGlobalCache::instance()
