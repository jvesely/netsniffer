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
	
	IRecognizer::QuickResult result;
	IRecognizer* worker = m_nextRecognizers[m_connection];

	if (!worker)
	{
		for( RecognizerList::ConstIterator it = m_recognizers.begin(); 
			it != m_recognizers.end(); ++it )
		{
			if ((*it)->quickLook( &result, m_connection.data() ))
			{
				m_nextRecognizers[m_connection] = *it;
				PRINT_DEBUG << "FOUND recognizer:" << (*it)->name() << "res" << result;
				// now store result somewhere and send signal that this field has changed
			}
		}
	} else {
		//change comment and send signal that it has been changed
		PRINT_DEBUG << "Analyzing connection using" << worker->name();
		PRINT_DEBUG << worker->quickLook( &result, m_connection.data() ) << result;
	}
}
