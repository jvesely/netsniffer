#include "Analyzer.h"
#include "gui/MainWindow.h"
#include "gui/uitexts.h"

int main(int argc, char** argv){
	try {
		QApplication app( argc, argv );
		
		QCoreApplication::setOrganizationName( UiTexts::ORGANIZATION_NAME );
		QCoreApplication::setOrganizationDomain( UiTexts::ORGANIZATION_DOMAIN );
		QCoreApplication::setApplicationName( UiTexts::APPLICATION_NAME );

		Analyzer::instance().loadSettings();
		MainWindow window( &Analyzer::instance() );
		window.show();
		return app.exec();
	} catch (...){
	 return -1;
	}
}
