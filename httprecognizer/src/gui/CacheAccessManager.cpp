#include "CacheAccessManager.h"
#include "HttpNetworkReply.h"
#include "Http.h"

#define DEBUG_TEXT "[ CacheAccessManager ]:"
#include "debug.h"

QNetworkReply* CacheAccessManager::createRequest ( Operation op, const QNetworkRequest& req, QIODevice* outgoingData )
{
	Q_ASSERT (mCache);
	PRINT_DEBUG( "operation:" << op << "url:" << req.url() << mCache->contains( req.url() ) );

	HttpNetworkReply* reply =  new HttpNetworkReply( req.url(), mCache->object( req.url() ) );
	Q_ASSERT (reply);
	return reply;
}
