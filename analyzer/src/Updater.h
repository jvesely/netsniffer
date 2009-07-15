/*!
 * @file
 * @brief Short description.
 *
 * Long description. I would paste some Loren Ipsum rubbish here, but I'm afraid
 * It would stay that way. Not that this comment is by any means ingenious but
 * at least people can understand it.
 */
#pragma once

#include "Connection.h"

class Updater:public QThread
{

public:
	Updater()
	{ timer.moveToThread( this );	};
	~Updater(){	quit();	wait( TIME_OUT ); terminate();	};

public slots: 

	void takeConnection( Connection::Pointer connection )
	{
		connection->moveToThread( this );
		const bool res = connect( &timer, SIGNAL(timeout()), connection.data(), SLOT(update()) );

		Q_UNUSED( res );
		Q_ASSERT( res );
	};
private:
	Q_OBJECT;

	QTimer timer;
	static const int TIME_OUT = 125;
	static const int SECOND = 1000;
	
	void run() { timer.start( SECOND ); exec();	};
};
