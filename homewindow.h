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

#include "titlewidget.h"
#include "leftwidget.h"
#include "contentwidget.h"
#include "righttopwidget.h"

class HomeWindow	:public QMainWindow
{
	 Q_OBJECT
public:
	static HomeWindow* instance();

	~HomeWindow();

public slots:
	// ע����¼
	void	logout();

	// ����վ��������վ��
	void	loadStationsById(int id,QString tname);

	// ��ͼ���ļ�
	void	openFile(QString fname,QString sname);
private:
	 HomeWindow(QWidget *parent = 0);
	void		initUi();

	QWidget*	initMidWidget();
	QWidget*	initRightWidet();

	QWidget*	initBottomWidget();

	// ��ʼ���źŲ�
	void		initConnections();

	// ��������
	void		loadData();
private:
	static HomeWindow*	m_inst;
	LeftWidget*		m_leftWidget;
	TitleWidget*	m_titleWidget;
	ContentWidget*	m_contentWidget;
	RightTopWidget*	m_rightTopWidget;
};

#endif