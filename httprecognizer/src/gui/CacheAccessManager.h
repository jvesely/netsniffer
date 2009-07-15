/*!
 * @file
 * @brief Short description.
 *
 * Long description. I would paste some Loren Ipsum rubbish here, but I'm afraid
 * It would stay that way. Not that this comment is by any means ingenious but
 * at least people can understand it.
 */
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
