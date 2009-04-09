#include "DropArea.h"

DropArea::DropArea( QWidget * parent )
:QScrollArea( parent )
{
	QVBoxLayout * verticalLayout = new QVBoxLayout( this );
	Q_ASSERT( verticalLayout );

	verticalLayout->setAlignment( Qt::AlignTop );
	setLayout( verticalLayout );
}
/*----------------------------------------------------------------------------*/
void DropArea::dragEnterEvent( QDragEnterEvent * event )
{
	Q_ASSERT( event );
	Q_ASSERT( event->mimeData() );

	if (event->mimeData()->hasText())
		event->accept();
};
/*----------------------------------------------------------------------------*/
void DropArea::dropEvent( QDropEvent * event )
{
	Q_ASSERT( event );
	Q_ASSERT( event->mimeData() );
	
	const QString path = event->mimeData()->text().remove("file://").trimmed();
	if ( QFile::exists(path))
		emit newPlugin( path );
}
