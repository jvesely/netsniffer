#pragma once

#include "Http.h"

class CacheAccessManager: public QNetworkAccessManager
{
public:
	CacheAccessManager( const Http::Cache* cache  ): mCache( cache ){};
	virtual QNetworkReply* createRequest ( Operation op, const QNetworkRequest& req, QIODevice* outgoingData = 0 );

private:
	const Http::Cache* mCache;
};
