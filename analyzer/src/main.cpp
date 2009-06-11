#include "Analyzer.h"
#include "gui/MainWindow.h"

int main(int argc, char** argv){
	try {
		QApplication app( argc, argv );
		
		QCoreApplication::setOrganizationName( "Student" );
		QCoreApplication::setOrganizationDomain( "student.mff" );
		QCoreApplication::setApplicationName( "IPAnalyzer" );

		Analyzer::instance().loadSettings();
		MainWindow window( &Analyzer::instance() );
		window.show();
		return app.exec();
	} catch (...){
	 return -1;
	}
}
