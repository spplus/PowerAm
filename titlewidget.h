/************************************************************************/
/* 
	DESC:	主页标题窗口类.
	DATE:	2016-05-09
	AUTHOR:	YUANLS
*/
/************************************************************************/

#ifndef __TITLEWIDGET_H__
#define __TITLEWIDGET_H__

#include <QtGui>

class TitleWidget	:public QMainWindow
{
	Q_OBJECT
public:

	explicit TitleWidget(QString username,QWidget *parent = 0);
	~TitleWidget();

	void setUserName(QString userName);

signals:
	void	logout();

private:
	void		initUi();
	QString		getDate();

private:
	QString		m_userName;
};

#endif