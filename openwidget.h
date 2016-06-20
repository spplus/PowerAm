/************************************************************************/
/* 
	DESC:	´ò¿ª´æµµ´°¿Ú.
	DATE:	2016-06-16
	AUTHOR:	YUANLS
*/
/************************************************************************/

#ifndef __OPENWIDGET_H__
#define __OPENWIDGET_H__

#include "define.h"
#include "buff/msgbody.pb.h"
#include <QtGui>

class OpenWidget		:public QDialog
{
	Q_OBJECT
public:
	OpenWidget(QWidget* parent = NULL);
	void		setData(PBNS::SavingListMsg_Response &res);
	int		getSaveId();
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

	PBNS::SavingListMsg_Response		m_dataList;
};

#endif