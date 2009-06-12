#include "ConnectionJob.h"
#include "IRecognizer.h"

#define DEBUG_TEXT "[ ConnectionJob ]:"
#include "debug.h"

/*----------------------------------------------------------------------------*/
void ConnectionJob::run()
{
	Q_ASSERT(m_connection);
	if (m_recognizers.count() == 0) {
		PRINT_DEBUG << "No recognizers..";
		return;
	}
	
	QVariant result;
	RecognizerTable& table = recognizerTable();
	IRecognizer* worker = table[m_connection.data()];

	if (!worker)
	{
		for( RecognizerList::ConstIterator it = m_recognizers.begin(); 
			it != m_recognizers.end(); ++it )
		{
			if ((*it)->parse( &result, m_connection.data() ))
			{
				table[m_connection.data()] = *it;
				PRINT_DEBUG << "FOUND recognizer:" << (*it)->name() << "res" << result;
				m_comments[ m_connection.data() ] = result;
				// now store result somewhere and send signal that this field has changed
			}
		}
	} else {
		//change comment and send signal that it has been changed
		PRINT_DEBUG << "Analyzing connection using" << worker->name();
		worker->parse( &result, m_connection.data() );
		m_comments[ m_connection.data() ] = result;
		PRINT_DEBUG << result;
	}
}
