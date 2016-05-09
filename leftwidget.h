/************************************************************************/
/* 
		DESC:	��ҳ���վ������б�.
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

	// ����վ�����
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