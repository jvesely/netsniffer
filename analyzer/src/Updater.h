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
		Q_ASSERT(res);
	};
private:
	Q_OBJECT;

	QTimer timer;
	static const int TIME_OUT = 125;
	static const int SECOND = 1000;
	
	void run() { timer.start( SECOND ); exec();	};
};
