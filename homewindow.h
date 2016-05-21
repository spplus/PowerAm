/************************************************************************/
/* 
	DESC:	��ҳ����.
	DATE:	2016-05-07
	AUTHOR:	YUANLS	
*/
/************************************************************************/

#ifndef __HOMEWINDOW_H__
#define __HOMEWINDOW_H__

#include <QMainWindow>
#include <QScrollArea>
#include <string>
#include "titlewidget.h"
#include "leftwidget.h"
#include "contentwidget.h"
#include "righttopwidget.h"

using namespace std;
class HomeWindow	:public QMainWindow
{
	 Q_OBJECT
public:
	static HomeWindow* instance();

	~HomeWindow();
	void	setUserName(QString username);

public slots:
	// ע����¼
	void	logout();

	// ����վ��������վ��
	void	loadStationsById(int id,QString tname);

	// ��ͼ���ļ�
	void	openFile(PBNS::StationBean& bean);

	// ��������
	void		recvdata(int msgtype,const char* msg,int msglength);

private:
	 HomeWindow(QWidget *parent = 0);
	void		initUi();

	QWidget*	initMidWidget();
	QWidget*	initRightWidet();

	QWidget*	initBottomWidget();

	// ��ʼ���źŲ�
	void		initConnections();

	//�û���¼������
	void		retUserMgr(int msgtype,const char* msg,int msglength);
private:

	static HomeWindow*	m_inst;
	LeftWidget*		m_leftWidget;
	TitleWidget*	m_titleWidget;
	ContentWidget*	m_contentWidget;
	RightTopWidget*	m_rightTopWidget;

	PBNS::StationTypeMsg_Response	m_stationList;


};

#endif