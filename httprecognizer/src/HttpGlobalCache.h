#pragma once

#include "Http.h"
#include "Singleton.h"

class HttpGlobalCache: public Http::Cache, public Singleton<HttpGlobalCache>
{};

#define HTTP_CACHE HttpGlobalCache::instance()
