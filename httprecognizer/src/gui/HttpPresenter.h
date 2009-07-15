/*!
 * @file
 * @brief Short description.
 *
 * Long description. I would paste some Loren Ipsum rubbish here, but I'm afraid
 * It would stay that way. Not that this comment is by any means ingenious but
 * at least people can understand it.
 */
#pragma once

#include "ui_HttpPresenter.h"

#include "HttpRecognizer.h"
#include "HttpConnectionModel.h"
#include "CacheAccessManager.h"

class HttpPresenter: public QDialog, private Ui::HttpPresenter
{
public:
	HttpPresenter( const HttpRecognizer::ConnectionTable& connections );

public slots:
	void selectResource( const QModelIndex& index );

private:
	HttpConnectionModel mModel;
	CacheAccessManager* mAccessManager;

	Q_OBJECT;
	Q_DISABLE_COPY( HttpPresenter );
	
};
