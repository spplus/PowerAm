/************************************************************************/
/* 
	DESC:	±£´æ´æµµ´°¿Ú.
	DATE:	2016-06-16
	AUTHOR:	YUANLS
*/
/************************************************************************/

#ifndef __SAVEWIDGET_H__
#define __SAVEWIDGET_H__

#include <QtGui>

class SaveWidget		:public QDialog
{
	Q_OBJECT
public:
	SaveWidget(QWidget* parent=NULL);
	QString		getSaveName();

private slots:
	void				onOk();

private:
	void		initUi();

private:
	QLineEdit*		m_saveName;
};


#endif