/*!
 * @file
 * @brief Short description.
 *
 * Long description. I would paste some Loren Ipsum rubbish here, but I'm afraid
 * It would stay that way. Not that this comment is by any means ingenious but
 * at least people can understand it.
 */
#pragma once

class DropArea: public QScrollArea
{
	Q_OBJECT;

public:
	DropArea( QWidget* parent = NULL );

signals:
	void newPlugin( const QString );

protected slots:
	void dragEnterEvent( QDragEnterEvent* );
	void dropEvent( QDropEvent* );
};

