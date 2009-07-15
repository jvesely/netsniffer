/*!
 * @file
 * @brief Short description.
 *
 * Long description. I would paste some Loren Ipsum rubbish here, but I'm afraid
 * It would stay that way. Not that this comment is by any means ingenious but
 * at least people can understand it.
 */
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
