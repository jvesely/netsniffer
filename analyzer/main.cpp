
#include <QVector>
#include <QDebug>
#include <QPointer>
#include <QPluginLoader>
#include <QThread>
#include <QCoreApplication>
#include "IDevice.h"
#include "IDevList.h"
#include "cAnalyzer.h"


int main(int argc, char** argv){

	QCoreApplication app(argc, argv);
	cAnalyzer * analyzer = new cAnalyzer();

	QPluginLoader loader("d:\\progs\\C\\netsniffer\\NetDump\\release\\NetDump.dll");
	qDebug()<<"Loading plugin ... "<< loader.load()<<endl;

	IDevList * list = qobject_cast<IDevList *>(loader.instance());

	QPointer<IDevice> eth;
	int i = list->getCount();
	int inum;
	if(!i){
		qDebug()<<"No interfaces found!\n";
		return -1;
	}
		

	printf("Enter the interface number (0-%d):",i-1);
	scanf_s("%d", &inum);
	eth = (*list)[inum];
	if(!eth)
	{
		qCritical()<<"Unable to open the adapter.\n";
		delete list;
		return -1;
	}
	printf("\nlistening on %s...\n", eth->getDesc().toStdString().c_str());
	
	QObject::connect(eth, SIGNAL(packetArrived(IDevice*, QByteArray *)), analyzer, SLOT(analyze(IDevice*, QByteArray *)));
	
	
	/* start the capture */
	eth->capture();
	return app.exec();
	//Sleep(1000);
	//while(1);
	//eth->stop();
	//Sleep(1000);
}