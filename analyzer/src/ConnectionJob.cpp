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
	
	IRecognizer* worker = m_connection->recognizer();

	if (!worker)
	{
		/* find suitable recognizer */
		for( RecognizerList::ConstIterator it = m_recognizers.begin(); 
			it != m_recognizers.end(); ++it )
		{
			if ((*it)->parse( m_connection.data() ))
			{
				m_connection->setRecognizer( *it );
				PRINT_DEBUG << "FOUND recognizer:" << (*it)->name();
			}
		}
	} else {
		PRINT_DEBUG << "Analyzing connection using" << worker->name();
		worker->parse( m_connection.data() );
	}
}
