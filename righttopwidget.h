/************************************************************************/
/* 
		DESC:	右上角窗口类，显示目前操作路径.
		DATE:	2016-05-09
		AUTHOR:	YUANLS
*/
/************************************************************************/
#ifndef __RIGHTTOPWIDGET_H__
#define __RIGHTTOPWIDGET_H__

#include <QtGui>

class RightTopWidget		:public QWidget
{
public:
	RightTopWidget(QWidget* parent = NULL);
	~RightTopWidget();

	// 设置操作路径
	void		setOppath(QString path);

	// 设置站点名称
	void		setStationName(QString sname);
private:
	void		initUi();

private:
	QString	m_oppath;
	QLabel*	m_label;
};



#endif
