#pragma once

class PluginLoader: public QPluginLoader
{
public:
	inline PluginLoader( const QString& file ):	QPluginLoader( file ) {};

	virtual ~PluginLoader()
		{ if (loaded()) unload();}

	inline bool loaded()
		{ return this->QPluginLoader::isLoaded(); };

	inline const QString fileName()
		{ return this->QPluginLoader::fileName(); };

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
