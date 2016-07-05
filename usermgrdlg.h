/************************************************************************/
/* 
	DESC:		�û�����
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

	void reqUserList();										    //�����û��б�
	void retUserManager(int msgtype,const char* msg);			//�û�����

private:
	
	void reqUserRoleList();										//�����û���ɫ�б�

	void retUserList(const char* msg);							//�õ������û��б�
	void retUserRoleList(const char* msg);						//�õ������û���ɫ�б�


signals:

public slots:
	void getUserItem(QTableWidgetItem* item);					//ѡ��tab����ĳһ����Ӧ����
	void addUser();
	void delUser();
	void mdfUser();

private:
	
	QLabel		*pnamelab,*prolelab,*ppwdlab,*prealnamelab;		//��ǩ:�û���,��ɫ,����,��ʵ����
	QLineEdit	*pnamelied,*ppwdlined,*prealnameld;				//�����:�û���,����,��ʵ����
	QComboBox	*prolecbx;										//��ɫ

	QTableWidget *ptabwdgulist;									//�û��б�
	QPushButton	*pbntadd,*pbntdel,*pbntmdf;						//������ť:���,ɾ��,�޸�

	bool		baddflag;										//����û���־
	bool		bmdfflag;										//�޸��û���־
	bool        bfst;
};

#endif // USERMGRDLG_H
