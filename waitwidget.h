/************************************************************************/
/* 
	DESC:	�ȴ����ڣ���ʾgifͼ��.
	DATE:	2016-05-16
	AUTHOR:	YUANLS
*/
/************************************************************************/

#ifndef __WAITWIDGET_H__
#define __WAITWIDGET_H__

#include<QPainter>
#include<QMovie>
#include <QtGui>

class WaitWidget	:public QDialog  
{
	Q_OBJECT
public:
	WaitWidget(QWidget*parent = NULL);
};



#endif