/************************************************************************/
/* 
		DESC:	�ұ߹�����վ���б���ʾ����.
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
	
	// ��ͼ��
	void	openfile(QString fname,QString sname);

public slots:
	
	// ����վ��
	void	loadData(int row=5,int col=4);

	// ��ť�����¼�
	void	btnPressed();
	
private:
	
	QGridLayout*	m_gbox;
	QWidget*		m_widget;
};

#endif
