/*!
 * @file
 * @brief Short description.
 *
 * Long description. I would paste some Loren Ipsum rubbish here, but I'm afraid
 * It would stay that way. Not that this comment is by any means ingenious but
 * at least people can understand it.
 */
#include "Analyzer.h"
#include "gui/MainWindow.h"
#include "gui/uitexts.h"

#define ARRAY_SIZE(array) (sizeof(array) / sizeof(array[0]))

int main(int argc, char** argv)
{
	try {
		QApplication app( argc, argv );

		{ /* Creating application icon. */
			QIcon icon;
			const int sizes[] = { 16, 32, 48 };

			for (uint i = 0; i < ARRAY_SIZE(sizes); ++i) {
				icon.addPixmap( QString( ":/icons/icon-%1.png" ).arg( sizes[i] ) );
			}

			qApp->setWindowIcon( icon );
		}
		
		QCoreApplication::setOrganizationName( UiTexts::ORGANIZATION_NAME );
		QCoreApplication::setOrganizationDomain( UiTexts::ORGANIZATION_DOMAIN );
		QCoreApplication::setApplicationName( UiTexts::APPLICATION_NAME );

		ANALYZER.loadSettings();
		MainWindow window( &ANALYZER );
		window.show();
		const int ret =  qApp->exec();
		ANALYZER.saveSettings();
		return ret;
	} catch (...) {
	 return -1;
	}
}
