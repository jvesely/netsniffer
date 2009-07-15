#pragma once

#include "ui_HttpPresenter.h"

#include "HttpRecognizer.h"
#include "HttpConnectionModel.h"

class HttpPresenter: public QDialog, private Ui::HttpPresenter
{
public:
	HttpPresenter( const ConnectionTable& connections );

private:
	HttpConnectionModel mModel;

	Q_DISABLE_COPY( HttpPresenter );
	
};
