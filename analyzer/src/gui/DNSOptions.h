#pragma once

#include "ui_CacheView.h"
#include "IOptionsPage.h"

class DNSCache;

class DNSOptions: public QObject, public Ui_CacheView, public IOptionsPage
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
	void remove( bool all);

private:
	Q_OBJECT;
	Q_DISABLE_COPY (DNSOptions);

	static const QString MY_NAME;
	DNSCache* m_dns;


};
