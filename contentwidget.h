/************************************************************************/
/* 
		DESC:	右边工作区站点列表显示区域.
		DATE:	2016-05-09
		AUTHOR:	YUANLS
*/
/************************************************************************/

#ifndef __CONTENTWIDGET_H__
#define __CONTENTWIDGET_H__

#include <QtGui>

class ContentWidget	:public QScrollArea
{
	Q_OBJECT
public:
	ContentWidget(QWidget* parent= NULL);
	~ContentWidget();

signals:
	
	// 打开图形
	void	openfile(QString fname);

public slots:
	
	// 加载站点
	void	loadData();

private:
	void	initUi();
	
private:
	QGridLayout*	m_gbox;

};

#endif
