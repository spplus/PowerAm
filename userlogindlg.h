/************************************************************************/
/* 
	DESC:		�û���¼����
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
	bool getCloselgn(){return bcloselgn;}			//���ص�¼����رձ�־
	QString getLoginUser();							//ȡ��¼�û���
	int		getLoginUserId();						//��ȡ��¼�û�ID
	int		getLoginRoleId();						//��ȡ��¼�û���ɫID
	QString getLoginPasswd();						//��ȡ��¼�û�����
	QString getLoginRoleName();						//��ȡ��¼��ɫ����
	QString getLoginRealName();						//��ȡ��¼�û���ʵ����
	void setNetflag(bool bNet){bnetflag = bNet;};	//д�������ӳɹ���־
	void	setLoginPasswd(QString pwd){passwd = pwd;};	//�����޸Ĺ����������
	

signals:

public slots:
	void recvdata(int msgtype,const char* msg,int msglength);			// ��������
	void closedlg();								//�ر�
	void login();									//�û���¼

protected:
	void mousePressEvent(QMouseEvent *event);		//�������Ӧ�¼�
	void mouseReleaseEvent(QMouseEvent *event);		//����ͷ���Ӧ�¼�
	void mouseMoveEvent(QMouseEvent *event);		//����ƶ���Ӧ�¼�

private:

	QWidget		*loginwidget;						//��¼����
	QWidget		*plgntitel_widget;					//��¼����widget
	QLabel		*ploginbklab;						//��¼����ʱʹ��
	//QComboBox	*pusercobx;							//�û�
	QLineEdit	*puserlined;						//�û�
	QLineEdit	*ppwdlined;							//�����
	QPushButton	*ploginbtn;							//��¼��ť
	PushButton	*close_button;						//�������Ͻǹرհ�ť

	QPoint move_point;								//�ƶ��ľ���
	bool mouse_press;								//����������

	bool bcloselgn;									//��¼����رձ�־
	bool bnetflag;									//�������ӳɹ���־

	int		userid;									//�û�ID
	QString uname;									//�û�����
	QString passwd;									//�û���¼����
	int		roleid;									//��ɫID
	QString rolename;								//��ɫ����
	QString realname;								//�û���ʵ����


	static UserLogindlg*	m_inst;
	
};

#endif // USERLOGINDLG_H
