#pragma once

#include "Analyzer.h"
#include "Connection.h"

class ConnectionJob: public QRunnable
{
public:
	ConnectionJob( const Connection::Pointer connection, const Analyzer::RecognizerList list	)
	:m_connection( connection ), m_recognizers( list )	{};

	void run();

private:
	const Connection::Pointer m_connection;
	const Analyzer::RecognizerList m_recognizers;
};
