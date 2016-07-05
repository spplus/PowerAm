/************************************************************************/
/* 
	DESC:		用户管理
	DATE:		2016-05-10
	AUTHOR:		Abelsky
*/
/************************************************************************/
#ifndef USERMGRDLG_H
#define USERMGRDLG_H

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

#include "netclient.h"
#include "..\common\include\commands.h"
#include "..\common\buff\msgbody.pb.h"

#define	PBNS com::spplus::buff

class UserMgrdlg : public QDialog
{
	Q_OBJECT

public:
	UserMgrdlg(QWidget *parent);
	~UserMgrdlg();

	void reqUserList();										    //请求用户列表
	void retUserManager(int msgtype,const char* msg);			//用户管理

private:
	
	void reqUserRoleList();										//请求用户角色列表

	void retUserList(const char* msg);							//得到返回用户列表
	void retUserRoleList(const char* msg);						//得到返回用户角色列表


signals:

public slots:
	void getUserItem(QTableWidgetItem* item);					//选中tab表中某一行响应函数
	void addUser();
	void delUser();
	void mdfUser();

private:
	
	QLabel		*pnamelab,*prolelab,*ppwdlab,*prealnamelab;		//标签:用户名,角色,密码,真实姓名
	QLineEdit	*pnamelied,*ppwdlined,*prealnameld;				//输入框:用户名,密码,真实姓名
	QComboBox	*prolecbx;										//角色

	QTableWidget *ptabwdgulist;									//用户列表
	QPushButton	*pbntadd,*pbntdel,*pbntmdf;						//操作按钮:添加,删除,修改

	bool		baddflag;										//添加用户标志
	bool		bmdfflag;										//修改用户标志
	bool        bfst;
};

#endif // USERMGRDLG_H
