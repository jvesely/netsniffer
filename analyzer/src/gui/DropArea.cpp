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
