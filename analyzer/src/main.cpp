#include "Analyzer.h"

int main(int argc, char** argv){
	try {
		Analyzer analyzer(argc, argv);
		return analyzer.exec();
	} catch (...){
	 return -1;
	}
}
