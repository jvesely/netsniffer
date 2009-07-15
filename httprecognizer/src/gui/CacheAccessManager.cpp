/*!
 * @file
 * @brief Short description.
 *
 * Long description. I would paste some Loren Ipsum rubbish here, but I'm afraid
 * It would stay that way. Not that this comment is by any means ingenious but
 * at least people can understand it.
 */
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
