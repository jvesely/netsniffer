#include "Analyzer.h"
#include "gui/MainWindow.h"
#include "gui/uitexts.h"

#define ARRAY_SIZE(array) (sizeof(array) / sizeof(array[0]))

int main(int argc, char** argv)
{
	try {
		QApplication app( argc, argv );

		{ /* Creating main window icon. */
			QIcon icon;
			const int sizes[] = { 16, 32, 48 };

			for (uint i = 0; i < ARRAY_SIZE(sizes); ++i) {
				icon.addPixmap( QString( ":/icons/icon-%1.png" ).arg( sizes[i] ) );
			}

			app.setWindowIcon( icon );
		}
		
		QCoreApplication::setOrganizationName( UiTexts::ORGANIZATION_NAME );
		QCoreApplication::setOrganizationDomain( UiTexts::ORGANIZATION_DOMAIN );
		QCoreApplication::setApplicationName( UiTexts::APPLICATION_NAME );

		Analyzer::instance().loadSettings();
		MainWindow window( &Analyzer::instance() );
		window.show();
		return app.exec();
	} catch (...) {
	 return -1;
	}
}
