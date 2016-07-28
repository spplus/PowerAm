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
	void	openfile(PBNS::StationBean& bean);

public slots:
	
	// 加载站点
	void	loadData(PBNS::StationTypeMsg_Response& res,int tpid);

	// 按钮触发事件
	void	btnPressed();
	
private:
	
	QGridLayout*	m_gbox;
	QWidget*		m_widget;

	PBNS::StationTypeMsg_Response m_stationList;
};

// 声明模板类型，可以用于信号槽
Q_DECLARE_METATYPE(PBNS::StationBean);

#endif
