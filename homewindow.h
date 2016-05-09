/************************************************************************/
/* 
	DESC:	主页窗口.
	DATE:	2016-05-07
	AUTHOR:	YUANLS	
*/
/************************************************************************/

#ifndef __HOMEWINDOW_H__
#define __HOMEWINDOW_H__

#include <QMainWindow>
#include <QScrollArea>

#include "titlewidget.h"
#include "leftwidget.h"
#include "contentwidget.h"

class HomeWindow	:public QMainWindow
{
	 Q_OBJECT
public:
	explicit HomeWindow(QWidget *parent = 0);
	~HomeWindow();

public slots:
	// 注销登录
	void	logout();

	// 根据站点类别加载站点
	void	loadStationsById(int id);

private:

	void		initUi();
	QWidget*	initTitleWidget();
	QWidget*	initLeftWidget();
	QWidget*	initMidWidget();
	QWidget*	initRightWidet();
	QWidget*	initRightTopWidget();
	QWidget*	initRightBottomWidget();
	QWidget*	initBottomWidget();

	// 初始化信号槽
	void		initConnections();

	// 加载数据
	void		loadData();
private:

	LeftWidget*		m_leftWidget;
	TitleWidget*	m_titleWidget;
	ContentWidget*	m_contentWidget;
	QScrollArea*	m_scrollArea;
};

#endif