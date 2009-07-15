#pragma once

#include "Analyzer.h"
#include "Connection.h"

class ConnectionJob: public QRunnable
{
public:
	ConnectionJob( const Connection::Pointer connection, const Analyzer::RecognizerList list	)
	: mConnection( connection ), mRecognizers( list )	{};

	void run();

private:
	const Connection::Pointer mConnection;
	const Analyzer::RecognizerList mRecognizers;
};
