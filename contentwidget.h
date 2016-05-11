/************************************************************************/
/* 
		DESC:	右边工作区站点列表显示区域.
		DATE:	2016-05-09
		AUTHOR:	YUANLS
*/
/************************************************************************/

#ifndef __CONTENTWIDGET_H__
#define __CONTENTWIDGET_H__

#include "comutil.h"
#include <QtGui>

class ContentWidget	:public QScrollArea
{
	Q_OBJECT
public:
	ContentWidget(QWidget* parent= NULL);
	~ContentWidget();

signals:
	
	// 打开图形
	void	openfile(QString fname,QString sname);

public slots:
	
	// 加载站点
	void	loadData(PBNS::StationListMsg_Response& res);

	// 按钮触发事件
	void	btnPressed();
	
private:
	TreeNode* makeNode(PBNS::StationBean & bean,bool islast=false);
private:
	
	QGridLayout*	m_gbox;
	QWidget*		m_widget;

	PBNS::StationListMsg_Response m_stationList;
};

#endif
