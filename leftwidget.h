/************************************************************************/
/* 
		DESC:	主页左侧站点分类列表.
		DATE:	2016-05-09
		AUTHOR:	YUANLS
*/
/************************************************************************/

#ifndef __LEFTWIDGET_H__
#define __LEFTWIDGET_H__


#include <QtGui>
#include "comutil.h"


class LeftWidget	:public QWidget
{
	Q_OBJECT
public:
	LeftWidget(QWidget* parent=NULL);
	~LeftWidget();

	// 加载站点分类
	void					loadData(PBNS::StationTypeMsg_Response& res);

private slots:
	void					currentItemChange(QListWidgetItem * current, QListWidgetItem * previous);

signals:
	void					loadStations(int stationType,QString typeName);
private:
	void					initUi();
	TreeNode*		makeNode(PBNS::StationTypeBean & bean,int count);

private:
	QListWidget*	m_list;

	PBNS::StationTypeMsg_Response m_typeList;
};

#endif