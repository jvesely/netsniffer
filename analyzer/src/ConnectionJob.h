#pragma once

#include "Analyzer.h"

class ConnectionJob: public QRunnable
{
public:
	ConnectionJob( const Connection* connection, AnalyzerTable& analyzer_table
		const AnalyzerList& analyzer_list );

	void run();

private:
	const Connection* m_connection;
	const AnalyzerList& m_analyzers;
	AnalyzerTable& m_usedAnalyzers;
}
