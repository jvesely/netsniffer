#include "ConnectionJob.h"
#include "IRecognizer.h"

#define DEBUG_TEXT "[ ConnectionJob ]:"
#include "debug.h"

/*----------------------------------------------------------------------------*/
void ConnectionJob::run()
{
	Q_ASSERT (mConnection);
	if (mRecognizers.count() == 0)
	{
		PRINT_DEBUG ("No recognizers..");
		return;
	}
	

	if (!mConnection->recognizer())
	{
		/* find suitable recognizer */
		for( Analyzer::RecognizerList::ConstIterator it = mRecognizers.begin(); 
			it != mRecognizers.end(); ++it )
		{
			if ((*it)->guess( mConnection.data() ))
			{
				mConnection->setRecognizer( *it );
				PRINT_DEBUG ("FOUND recognizer:" << (*it)->name());
				break;
			}
		}

		if (!mConnection->recognizer())
		{
			PRINT_DEBUG ("Failed to find recognizer");
			return;
		}
	} 

	IRecognizer* worker = mConnection->recognizer();
	PRINT_DEBUG ("Analyzing connection using" << worker->name());
	worker->parse( mConnection.data() );
}
