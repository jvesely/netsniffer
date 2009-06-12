#pragma once

#include "Analyzer.h"
#include "Connection.h"

class ConnectionJob: public QRunnable
{
public:
	ConnectionJob(
		const ConnectionPtr connection,
		const RecognizerList recognizer_list,
		CommentStore& comments
		)
	:m_connection( connection ), 
	m_recognizers( recognizer_list ),
//	m_nextRecognizers( recognizer_table ),
	m_comments( comments )
	{};

	void run();
	static RecognizerTable& recognizerTable()
		{ static RecognizerTable recognizers; return recognizers; };

private:
//	static RecognizerTable m_nextRecognizers;

	const ConnectionPtr m_connection;
	const RecognizerList m_recognizers;
	CommentStore& m_comments;
};
