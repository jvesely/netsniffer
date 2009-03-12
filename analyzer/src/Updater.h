#pragma once

#include "Connection.h"

#define SECOND 1000
#define TIME_OUT 125

class Updater:public QThread{

	Q_OBJECT;
	
	QTimer timer;
	
	void run(){ timer.start(SECOND); exec();	};

public:
	Updater(){
		timer.moveToThread(this);
//		connect(&timer, SIGNAL(timeout()), this, SIGNAL(update()), Qt::DirectConnection);
		};
	~Updater(){	quit();	wait(TIME_OUT); terminate();	};
public slots: 
	void takeConnection(Connection * conn){
		return;
		conn->moveToThread(this);
		connect(&timer, SIGNAL(timeout()), conn, SLOT(update()));
	};
private:
signals:
	void update();

	

};
