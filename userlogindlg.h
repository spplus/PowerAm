/************************************************************************/
/* 
	DESC:		用户登录窗口
	DATE:		2016-04-30
	AUTHOR:		Abelsky
*/
/************************************************************************/

#ifndef USERLOGINDLG_H
#define USERLOGINDLG_H

#include <QObject>
#include <QtGui>
#include <QDialog>
#include <QWidget>
#include <QTabWidget>
#include <QPushButton>
#include <QTextEdit>
#include <QComboBox>
#include <QLineEdit>
#include <QStringList>
#include <QPainter>
#include <QMouseEvent>
#include <qmath.h>
#include "comutil.h"
#include "netclient.h"
#include "push_button.h"

class UserLogindlg : public QDialog
{
	Q_OBJECT

public:
	static UserLogindlg* instance();

	UserLogindlg();
	~UserLogindlg();

public:
	bool getCloselgn(){return bcloselgn;}			//返回登录界面关闭标志
	QString getLoginUser();							//取登录用户名
	int		getLoginUserId();						//获取登录用户ID
	int		getLoginRoleId();						//获取登录用户角色ID
	QString getLoginPasswd();						//获取登录用户密码
	QString getLoginRoleName();						//获取登录角色名称
	QString getLoginRealName();						//获取登录用户真实姓名
	void setNetflag(bool bNet){bnetflag = bNet;};	//写网络连接成功标志
	void	setLoginPasswd(QString pwd){passwd = pwd;};	//设置修改过后的新密码
	

signals:

public slots:
	void recvdata(int msgtype,const char* msg,int msglength);			// 接收数据
	void closedlg();								//关闭
	void login();									//用户登录

protected:
	void mousePressEvent(QMouseEvent *event);		//鼠标点击响应事件
	void mouseReleaseEvent(QMouseEvent *event);		//鼠标释放响应事件
	void mouseMoveEvent(QMouseEvent *event);		//鼠标移动响应事件

private:

	QWidget		*loginwidget;						//登录界面
	QWidget		*plgntitel_widget;					//登录标题widget
	QLabel		*ploginbklab;						//登录背景时使用
	//QComboBox	*pusercobx;							//用户
	QLineEdit	*puserlined;						//用户
	QLineEdit	*ppwdlined;							//密码框
	QPushButton	*ploginbtn;							//登录按钮
	PushButton	*close_button;						//界面右上角关闭按钮

	QPoint move_point;								//移动的距离
	bool mouse_press;								//按下鼠标左键

	bool bcloselgn;									//登录界面关闭标志
	bool bnetflag;									//网络连接成功标志

	int		userid;									//用户ID
	QString uname;									//用户名称
	QString passwd;									//用户登录密码
	int		roleid;									//角色ID
	QString rolename;								//角色名称
	QString realname;								//用户真实姓名


	static UserLogindlg*	m_inst;
	
};

#endif // USERLOGINDLG_H
