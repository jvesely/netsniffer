#pragma once

#include "ui_CacheView.h"
#include "IOptionsTab.h"
#include "DNSCacheModel.h"

class DNSOptions: public QObject, public Ui_CacheView, public IOptionsTab
{
public:
	DNSOptions( DNSCache* dns ): m_dns( dns ) {};
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

private:
	Q_OBJECT;
	Q_DISABLE_COPY (DNSOptions);

	static const QString MY_NAME;
	DNSCache* m_dns;
	QPointer<DNSCacheModel> m_model;


};
