#pragma once

class PluginLoader: public QPluginLoader
{
public:
	inline PluginLoader( const QString& file ):	QPluginLoader( file ) {};

	virtual ~PluginLoader()
		{ if (isLoaded()) unload();}

public slots:
	bool init();
	void load();
	void unload();

signals:
	void statusChanged( bool loaded );

private:
	Q_OBJECT;
	Q_DISABLE_COPY( PluginLoader );

};
