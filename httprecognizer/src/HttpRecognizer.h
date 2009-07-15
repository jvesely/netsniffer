/*!
 * @file
 * @brief Short description.
 *
 * Long description. I would paste some Loren Ipsum rubbish here, but I'm afraid
 * It would stay that way. Not that this comment is by any means ingenious but
 * at least people can understand it.
 */
#pragma once

#include "IRecognizer.h"
#include "NetworkInfo.h"
#include "struct/SafeHash.h"
#include "HttpConnection.h"


class HttpRecognizer: public IRecognizer
{
public:
	typedef SafeHash<NetworkInfo, HttpConnection> ConnectionTable;

	const QString name() const {return "HttpRecognizer"; };
	bool guess( const IConnection* connection );
	bool parse( IConnection* connection );
	QVariant comment( IConnection* connection );
	bool showDetails( IConnection* connection );

private:
	ConnectionTable mConnections;
};
