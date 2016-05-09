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
#include "righttopwidget.h"

class HomeWindow	:public QMainWindow
{
	 Q_OBJECT
public:
	static HomeWindow* instance();

	~HomeWindow();

public slots:
	// 注销登录
	void	logout();

	// 根据站点类别加载站点
	void	loadStationsById(int id,QString tname);

	// 打开图形文件
	void	openFile(QString fname,QString sname);
private:
	 HomeWindow(QWidget *parent = 0);
	void		initUi();

	QWidget*	initMidWidget();
	QWidget*	initRightWidet();

	QWidget*	initBottomWidget();

	// 初始化信号槽
	void		initConnections();

	// 加载数据
	void		loadData();
private:
	static HomeWindow*	m_inst;
	LeftWidget*		m_leftWidget;
	TitleWidget*	m_titleWidget;
	ContentWidget*	m_contentWidget;
	RightTopWidget*	m_rightTopWidget;
};

#endif