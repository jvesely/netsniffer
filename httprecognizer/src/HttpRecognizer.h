#pragma once

#include "IRecognizer.h"

class HttpRecognizer: public IRecognizer
{
	const QString name() const {return "HttpRecognizer"; };
	bool guess( const IConnection* connection );
	bool parse( IConnection* connection );
	QVariant comment( IConnection* connection );
	bool showDetails( IConnection* connection );
};
