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
	void	openfile(QString fname);

public slots:
	
	// ����վ��
	void	loadData();

private:
	void	initUi();
	
private:
	QGridLayout*	m_gbox;

};

#endif
