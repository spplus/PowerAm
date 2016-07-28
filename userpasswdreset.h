/************************************************************************/
/* 
	DESC:		登录用户密码重置
	DATE:		2016-05-29
	AUTHOR:		Abelsky
*/
/************************************************************************/

#ifndef USERPASSWDRESET_H
#define USERPASSWDRESET_H

#include <QObject>
#include <QtGui>
#include <QDialog>
#include <QWidget>
#include <QPushButton>
#include <QTextEdit>
#include <QLineEdit>
#include "structs.h"
#include "comutil.h"
#include "userlogindlg.h"
#include "netclient.h"


class UserPasswdReset : public QDialog
{
	Q_OBJECT

public:
	UserPasswdReset(QWidget *parent);
	~UserPasswdReset();

	//密码修改返回
	void	retpasswdMgr(int msgtype,const char* msg);
signals:

private slots:

	//确认修改密码
	void	commitPwd();

private:
	QLineEdit	*poldpwd,*pnewpwd,*pnewagpwd;				//用户原密码，新密码，再次新密码
	QPushButton	*pbntpwdok;									//修改密码确认
	QString		qstrpwdmd5;									//保存修改后的密码
	
};

#endif // USERPASSWDRESET_H
