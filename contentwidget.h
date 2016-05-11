/************************************************************************/
/* 
		DESC:	�ұ߹�����վ���б���ʾ����.
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
	
	// ��ͼ��
	void	openfile(QString fname,QString sname);

public slots:
	
	// ����վ��
	void	loadData(PBNS::StationListMsg_Response& res);

	// ��ť�����¼�
	void	btnPressed();
	
private:
	TreeNode* makeNode(PBNS::StationBean & bean,bool islast=false);
private:
	
	QGridLayout*	m_gbox;
	QWidget*		m_widget;

	PBNS::StationListMsg_Response m_stationList;
};

#endif
