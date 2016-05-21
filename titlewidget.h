/************************************************************************/
/* 
	DESC:	主页标题窗口类.
	DATE:	2016-05-09
	AUTHOR:	YUANLS
*/
/************************************************************************/

#ifndef __TITLEWIDGET_H__
#define __TITLEWIDGET_H__

#include <QtGui>
#include "usermgrdlg.h"

class TitleWidget	:public QMainWindow
{
	Q_OBJECT
public:

	explicit TitleWidget(QWidget *parent = 0);
	~TitleWidget();

	void setUserName(QString userName);

	//用户登录管理返回
	void		retUserMgr(int msgtype,const char* msg,int msglength);

signals:
	void	logout();

private slots:

	// 用户管理
	void	userMgr();

	// 电站分类管理
	void	typeMgr();

	// 电站编辑
	void	stationMgr();

	// 规则库编辑
	void	roleMgr();

private:
	void		initUi();
	void		createMenu();
	QString		getDate();
	 
private:
	QLabel*			m_userName;
	QMenu*			m_menu;
	QAction*		m_userAction;
	QAction*		m_typeAction;
	QAction*		m_stationAction;
	QAction*		m_roleAction;

	QToolButton*	m_logout;
	QToolButton*	m_setting;

	//用户管理
	UserMgrdlg		*m_pUserMgrdlg;

};

#endif