/************************************************************************/
/* 
	DESC:		��¼�û���������
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

	//�����޸ķ���
	void	retpasswdMgr(int msgtype,const char* msg);
signals:

private slots:

	//ȷ���޸�����
	void	commitPwd();

private:
	QLineEdit	*poldpwd,*pnewpwd,*pnewagpwd;				//�û�ԭ���룬�����룬�ٴ�������
	QPushButton	*pbntpwdok;									//�޸�����ȷ��
	QString		qstrpwdmd5;									//�����޸ĺ������
	
};

#endif // USERPASSWDRESET_H
