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
	void	openfile(QString fname,QString sname);

public slots:
	
	// 加载站点
	void	loadData(int row=5,int col=4);

	// 按钮触发事件
	void	btnPressed();
	
private:
	
	QGridLayout*	m_gbox;
	QWidget*		m_widget;
};

#endif
