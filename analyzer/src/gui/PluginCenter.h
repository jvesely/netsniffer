#pragma once

#include "ui_PluginCenter.h"
#include "IOptionsPage.h"
#include "PluginLoader.h"


class PluginCenter: public QObject, public Ui_PluginCenter, public IOptionsPage
{
	Q_OBJECT;

public:
	PluginCenter() {};
	~PluginCenter() {};

	inline const QString name() const
		{ return MYNAME; }

	inline const QIcon* icon() const
	{
		static const QIcon ICON(QPixmap(QString::fromUtf8(":/net/dialog-information.png")));
		return &ICON;
	}

	bool deploy( QWidget* container );

public slots:
	void addPluginControl( PluginLoader* loader );

signals:
	void newPlugin( QString file );
	void deleteAll();

protected slots:
	void addPlugin();
	void removeAll();
	void dropEvent(QDropEvent* event);
	void dragEnterEvent(QDragEnterEvent * event);

private:
	Q_DISABLE_COPY (PluginCenter);
	
 	static const QString MYNAME;
};
