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

