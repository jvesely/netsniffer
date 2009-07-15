/*!
 * @file
 * @brief Short description.
 *
 * Long description. I would paste some Loren Ipsum rubbish here, but I'm afraid
 * It would stay that way. Not that this comment is by any means ingenious but
 * at least people can understand it.
 */
#pragma once

#include "ui_RecognizerLoad.h"
#include "PluginLoader.h"


class Control:public QWidget, private Ui::Control
{
public:
	Control( QWidget* parent, PluginLoader* loader );
	~Control() {};

public slots:
	void updateStatus( bool loaded );
	void switchStatus();

private:
	Q_OBJECT

	QPointer<PluginLoader> mPlugin;
};
