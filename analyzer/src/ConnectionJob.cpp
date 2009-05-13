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
	if (!m_nextRecognizers[m_connection])
	{
		m_nextRecognizers[m_connection] = m_recognizers.first();
	}
	IRecognizer* worker = m_nextRecognizers[m_connection];

	Q_ASSERT(worker);
	PRINT_DEBUG << "Analyzing connection using" << worker->name();
	IRecognizer::QuickResult result;
	PRINT_DEBUG << worker->quickLook( &result, m_connection.data() ) << result;
//	QPointer<IRecognizer> next_recognizer =
//		m_usedRecognizers;
}
