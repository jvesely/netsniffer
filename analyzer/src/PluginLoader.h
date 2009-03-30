#pragma once

class PluginLoader: public QPluginLoader
{

Q_OBJECT;

public:
	PluginLoader( QString file );
	inline bool loaded()
		{ return this->QPluginLoader::isLoaded(); };

	inline const QString fileName()
		{ return this->QPluginLoader::fileName(); };

public slots:
	bool init();
	void load();
	void unload();

	void deleteLater()
		{ this->QObject::deleteLater(); };

signals:
	void statusChanged( bool loaded );

private:
	Q_DISABLE_COPY(PluginLoader);

};
