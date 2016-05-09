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

class LeftWidget	:public QWidget
{
	Q_OBJECT
public:
	LeftWidget(QWidget* parent=NULL);
	~LeftWidget();

	// 加载站点分类
	void	loadData();

private slots:
	void	currentItemChange(QListWidgetItem * current, QListWidgetItem * previous);

signals:
	void	loadStations(int stationType);
private:
	void initUi();

private:
	QListWidget*	m_list;
};

#endif