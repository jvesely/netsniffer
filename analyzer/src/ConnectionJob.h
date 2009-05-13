#pragma once

#include "Analyzer.h"
#include "Connection.h"

class ConnectionJob: public QRunnable
{
public:
	ConnectionJob( const ConnectionPtr connection,
		RecognizerTable& recognizer_table, const RecognizerList& recognizer_list )
	:m_connection( connection ), m_recognizers( recognizer_list ),
	m_nextRecognizers( recognizer_table ) {};

	void run();

private:
	const ConnectionPtr m_connection;
	const RecognizerList& m_recognizers;
	RecognizerTable& m_nextRecognizers;
};
