#pragma once

#include "IConnection.h"
#include "ui_AnalyzeDialog.h"

class AnalyzeDialog:public QDialog, protected Ui::AnalyzeDialog {
	Q_OBJECT;
	
	QPointer<IConnection> connection;

	
	AnalyzeDialog();
	const AnalyzeDialog& operator=(const AnalyzeDialog& other);
public:
	AnalyzeDialog(QWidget * parent, IConnection * con);
	inline void setInterior(QWidget * interior)
		 {
	//		 qDebug() <<  "Inserted Widget" << interior;

			 verticalLayout->insertWidget(1, interior); 
		//	 qDebug() <<  "Inserted Widget";

		//	 interior->show();  
			//repaint();
			}; 
};
