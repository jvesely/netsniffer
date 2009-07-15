#pragma once

#include "IRecognizer.h"
#include "NetworkInfo.h"
#include "struct/SafeHash.h"
#include "HttpConnection.h"

typedef SafeHash<NetworkInfo, HttpConnection> ConnectionTable;

class HttpRecognizer: public IRecognizer
{
public:
	const QString name() const {return "HttpRecognizer"; };
	bool guess( const IConnection* connection );
	bool parse( IConnection* connection );
	QVariant comment( IConnection* connection );
	bool showDetails( IConnection* connection );

private:
	ConnectionTable mConnections;
	
};
