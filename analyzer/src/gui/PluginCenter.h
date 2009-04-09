#pragma once

#include "ui_PluginCenter.h"
#include "IOptionsPage.h"
#include "PluginLoader.h"
#include "DropArea.h"

typedef QList<DropArea*> AreaList;

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
	void removeArea( QObject* area );

private:
	Q_DISABLE_COPY (PluginCenter);
	
 	static const QString MYNAME;

	AreaList m_areas;
};
