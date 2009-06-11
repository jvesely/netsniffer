#include "Analyzer.h"
#include "gui/MainWindow.h"

int main(int argc, char** argv){
	try {
		Analyzer analyzer(argc, argv);
		MainWindow window( &analyzer );
		return analyzer.exec();
	} catch (...){
	 return -1;
	}
}
