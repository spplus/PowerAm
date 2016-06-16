/************************************************************************/
/* 
	DESC:	�򿪴浵����.
	DATE:	2016-06-16
	AUTHOR:	YUANLS
*/
/************************************************************************/

#ifndef __OPENWIDGET_H__
#define __OPENWIDGET_H__

#include <QtGui>

class OpenWidget		:public QDialog
{
	Q_OBJECT
public:
	OpenWidget(QWidget* parent = NULL);

private slots:
	void		itemDoubleClicked ( QTableWidgetItem * item );

private:
	void		initUi();
	void		initData();
	void		initTable();
	void		initSlots();
private:
	int					m_saveId;
	QPushButton*			m_okBtn;
	QPushButton*			m_cancelBtn;
	QTableWidget*		m_saveList;
};

#endif