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
#include "userlogindlg.h"
#include "userpasswdreset.h"
#include "usermgrdlg.h"
#include "stationmgr.h"
#include "rulemgrdlg.h"

class TitleWidget	:public QMainWindow
{
	Q_OBJECT
public:

	explicit TitleWidget(QWidget *parent = 0);
	~TitleWidget();

	void setUserName(QString userName);

	//用户登录管理返回
	void		retUserMgr(int msgtype,const char* msg,int msglength);

	//规则编辑管理返回
	void		retRuleMgr(int msgtype,const char* msg);

	//厂站管理返回
	void		retStationMgr(int msgtype,const char* msg);

	//密码修改返回
	void		retpasswdMgr(int msgtype,const char* msg);

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

	//用户密码重置管理
	void	passwdMgr();


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
	QToolButton*	m_userpwd;

	//用户管理
	UserMgrdlg		*m_pUserMgrdlg;
	//登录用户密码管理
	UserPasswdReset *m_pUserPwddlg;
	//厂站管理
	StationMgr		*m_pStationMgrdlg;
	//规则编辑管理
	RuleMgrdlg		*m_pRuleMgrdlg;

	PBNS::StationTypeMsg_Response	m_stationList;
};

#endif