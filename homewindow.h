/************************************************************************/
/* 
	DESC:	Ö÷Ò³´°¿Ú.
	DATE:	2016-05-07
	AUTHOR:	YUANLS	
*/
/************************************************************************/

#ifndef __HOMEWINDOW_H__
#define __HOMEWINDOW_H__

#include <QMainWindow>
#include <QScrollArea>

class HomeWindow	:public QMainWindow
{
	 Q_OBJECT
public:
	explicit HomeWindow(QWidget *parent = 0);
	~HomeWindow();

public slots:
	
private:

	void		initUi();
	QWidget*	initTitleWidget();
	QWidget*	initLeftWidget();
	QWidget*	initMidWidget();
	QWidget*	initRightWidet();
	QWidget*	initRightTopWidget();
	QWidget*	initRightBottomWidget();
	QWidget*	initBottomWidget();
private:

	QScrollArea*	m_scrollArea;
};

#endif