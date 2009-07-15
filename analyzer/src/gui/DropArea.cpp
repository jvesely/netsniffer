/*!
 * @file
 * @brief Short description.
 *
 * Long description. I would paste some Loren Ipsum rubbish here, but I'm afraid
 * It would stay that way. Not that this comment is by any means ingenious but
 * at least people can understand it.
 */
#include "DropArea.h"

DropArea::DropArea( QWidget* parent )
:QScrollArea( parent )
{
	QVBoxLayout* verticalLayout = new QVBoxLayout( this );
	Q_ASSERT( verticalLayout );

	verticalLayout->setAlignment( Qt::AlignTop );
	setLayout( verticalLayout );
	setAcceptDrops( true );
}
/*----------------------------------------------------------------------------*/
void DropArea::dragEnterEvent( QDragEnterEvent* event )
{
	Q_ASSERT( event );
	Q_ASSERT( event->mimeData() );

	if (event->mimeData()->hasUrls())
		event->accept();
};
/*----------------------------------------------------------------------------*/
void DropArea::dropEvent( QDropEvent* event )
{
	Q_ASSERT( event );
	Q_ASSERT( event->mimeData() );

	Q_FOREACH (QUrl url, event->mimeData()->urls())
	{
		const QString path = url.toLocalFile();
		if (QFile::exists(path)) {
			emit newPlugin( path );
		}
	}
}
