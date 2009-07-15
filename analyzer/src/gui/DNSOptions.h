#pragma once

#include "ui_CacheView.h"
#include "IOptionsTab.h"
#include "DNSCacheModel.h"

class DNSOptions: public QObject, public IOptionsTab, private Ui::CacheView
{
public:
	DNSOptions( DNSCache* dns ): mDns( dns ) {};
	~DNSOptions() {};

	inline const QString name() const
		{ return MY_NAME; };
	
	inline const QIcon* icon() const
	{
		static const QIcon ICON(QPixmap(QString::fromUtf8(":/net/dialog-information.png")));
		return &ICON;
	}

	bool deploy( QWidget* container );

private slots:
	void remove( int all = 1 );
	void refreshIndicator();

signals:
	void cacheMax( int );
	void cacheCount( int );

private:
	Q_OBJECT;
	Q_DISABLE_COPY (DNSOptions);

	static const QString MY_NAME;
	DNSCache* mDns;
	QPointer<DNSCacheModel> mModel;


};
