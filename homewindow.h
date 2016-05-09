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

class HomeWindow	:public QMainWindow
{
	 Q_OBJECT
public:
	explicit HomeWindow(QWidget *parent = 0);
	~HomeWindow();

public slots:
	// ע����¼
	void	logout();

	// ����վ��������վ��
	void	loadStationsById(int id);

private:

	void		initUi();
	QWidget*	initTitleWidget();
	QWidget*	initLeftWidget();
	QWidget*	initMidWidget();
	QWidget*	initRightWidet();
	QWidget*	initRightTopWidget();
	QWidget*	initRightBottomWidget();
	QWidget*	initBottomWidget();

	// ��ʼ���źŲ�
	void		initConnections();

	// ��������
	void		loadData();
private:

	LeftWidget*		m_leftWidget;
	TitleWidget*	m_titleWidget;
	ContentWidget*	m_contentWidget;
	QScrollArea*	m_scrollArea;
};

#endif