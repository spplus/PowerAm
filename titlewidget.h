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
#include "usermgrdlg.h"

class TitleWidget	:public QMainWindow
{
	Q_OBJECT
public:

	explicit TitleWidget(QWidget *parent = 0);
	~TitleWidget();

	void setUserName(QString userName);

	//�û���¼������
	void		retUserMgr(int msgtype,const char* msg,int msglength);

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

	//�û�����
	UserMgrdlg		*m_pUserMgrdlg;

};

#endif