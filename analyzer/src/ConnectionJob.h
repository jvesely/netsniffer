#pragma once

#include "Analyzer.h"

class ConnectionJob: public QRunnable
{
public:
	ConnectionJob( const QSharedPointer<Connection> connection,
		RecognizerTable& recognizer_table, const RecognizerList& recognizer_list )
	:m_connection( connection ), m_recognizers( recognizer_list ),
	m_usedRecognizers( recognizer_table ) {};

	void run()
	{ qDebug() << "Analyzing connection"; };

private:
	const QSharedPointer<Connection> m_connection;
	const RecognizerList& m_recognizers;
	RecognizerTable& m_usedRecognizers;
};
