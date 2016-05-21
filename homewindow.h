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
#include <string>
#include "titlewidget.h"
#include "leftwidget.h"
#include "contentwidget.h"
#include "righttopwidget.h"

using namespace std;
class HomeWindow	:public QMainWindow
{
	 Q_OBJECT
public:
	static HomeWindow* instance();

	~HomeWindow();
	void	setUserName(QString username);

public slots:
	// 注销登录
	void	logout();

	// 根据站点类别加载站点
	void	loadStationsById(int id,QString tname);

	// 打开图形文件
	void	openFile(PBNS::StationBean& bean);

	// 接收数据
	void		recvdata(int msgtype,const char* msg,int msglength);

private:
	 HomeWindow(QWidget *parent = 0);
	void		initUi();

	QWidget*	initMidWidget();
	QWidget*	initRightWidet();

	QWidget*	initBottomWidget();

	// 初始化信号槽
	void		initConnections();

	//用户登录管理返回
	void		retUserMgr(int msgtype,const char* msg,int msglength);
private:

	static HomeWindow*	m_inst;
	LeftWidget*		m_leftWidget;
	TitleWidget*	m_titleWidget;
	ContentWidget*	m_contentWidget;
	RightTopWidget*	m_rightTopWidget;

	PBNS::StationTypeMsg_Response	m_stationList;


};

#endif