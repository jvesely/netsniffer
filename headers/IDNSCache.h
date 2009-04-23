#pragma once

class IDNSCache: public QObject
{
	Q_OBJECT;

public slots:
	virtual bool insert( const QHostAddress address, const QString name ) = 0;
	virtual const QString translate( QHostAddress ) const = 0;

signals:
	virtual void newEntry( const QHostAddress address, const QString name );
};
