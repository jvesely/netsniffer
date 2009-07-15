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
