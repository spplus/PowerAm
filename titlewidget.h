/************************************************************************/
/* 
	DESC:	��ҳ���ⴰ����.
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

	//�û���¼������
	void		retUserMgr(int msgtype,const char* msg,int msglength);

	//����༭������
	void		retRuleMgr(int msgtype,const char* msg);

	//��վ������
	void		retStationMgr(int msgtype,const char* msg);

	//�����޸ķ���
	void		retpasswdMgr(int msgtype,const char* msg);

signals:
	void	logout();

private slots:

	// �û�����
	void	userMgr();

	// ��վ�������
	void	typeMgr();

	// ��վ�༭
	void	stationMgr();

	// �����༭
	void	roleMgr();

	//�û��������ù���
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

	//�û�����
	UserMgrdlg		*m_pUserMgrdlg;
	//��¼�û��������
	UserPasswdReset *m_pUserPwddlg;
	//��վ����
	StationMgr		*m_pStationMgrdlg;
	//����༭����
	RuleMgrdlg		*m_pRuleMgrdlg;

	PBNS::StationTypeMsg_Response	m_stationList;
};

#endif