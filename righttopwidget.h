/************************************************************************/
/* 
		DESC:	���ϽǴ����࣬��ʾĿǰ����·��.
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

	// ���ò���·��
	void		setOppath(QString path);

	// ����վ������
	void		setStationName(QString sname);
private:
	void		initUi();

private:
	QString	m_oppath;
	QLabel*	m_label;
};



#endif
