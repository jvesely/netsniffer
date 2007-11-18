#include "PcapList.h"
#include <QVector.h>
#include <QDebug.h>
#include <QPointer>

int main(void){
	//PcapList list;
	IDevList * list = new PcapList();
	QPointer<IDevice> eth;
	int i = list->getCount();
	int inum;
	if(!i){
		qDebug()<<"No interfaces found!\n";
		return -1;
	}
		

	printf("Enter the interface number (0-%d):",i-1);
	scanf_s("%d", &inum);

	eth = list->getInterface(inum);
	if(!eth)
	{
		qCritical()<<"Unable to open the adapter.\n";
		delete list;
		return -1;
	}
	printf("\nlistening on %s...\n", eth->getDesc().toStdString().c_str());
	
	
	
	/* start the capture */
	eth->capture();
	Sleep(1000);
	eth->stop();
	Sleep(1000);

	return 0;
}