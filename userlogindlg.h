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

#include "netclient.h"
#include "push_button.h"
#include "include\commands.h"
#include "buff\msgbody.pb.h"

#define		PBNS	com::spplus::buff

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
	void setNetflag(bool bNet){bnetflag = bNet;}	//д�������ӳɹ���־
	

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
	QComboBox	*pusercobx;							//�û�
	QLineEdit	*ppwdlined;							//�����
	QPushButton	*ploginbtn;							//��¼��ť
	PushButton	*close_button;						//�������Ͻǹرհ�ť

	QPoint move_point;								//�ƶ��ľ���
	bool mouse_press;								//����������

	bool bcloselgn;									//��¼����رձ�־
	bool bnetflag;									//�������ӳɹ���־

	static UserLogindlg*	m_inst;
	
};

#endif // USERLOGINDLG_H
