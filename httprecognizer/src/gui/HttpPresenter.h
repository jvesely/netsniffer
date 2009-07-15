#pragma once

#include "ui_HttpPresenter.h"

#include "HttpRecognizer.h"
#include "HttpConnectionModel.h"

class HttpPresenter: public QDialog, private Ui::HttpPresenter
{
public:
	HttpPresenter( const ConnectionTable& connections );

public slots:
	void selectResource( const QModelIndex& index );

private:
	HttpConnectionModel mModel;

	Q_OBJECT;
	Q_DISABLE_COPY( HttpPresenter );
	
};
