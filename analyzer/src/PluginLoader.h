/*!
 * @file
 * @brief Short description.
 *
 * Long description. I would paste some Loren Ipsum rubbish here, but I'm afraid
 * It would stay that way. Not that this comment is by any means ingenious but
 * at least people can understand it.
 */
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
