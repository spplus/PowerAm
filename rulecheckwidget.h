/************************************************************************/
/* 
	DESC:	����У�鴰�ڣ���ʾ�Ѵ����Ĺ����б�.
	DATE:	2016-06-25
	AUTHOR:	YUANLS
*/
/************************************************************************/

#ifndef __RULECHECKWIDGET_H__
#define __RULECHECKWIDGET_H__

#include "define.h"
#include "buff/msgbody.pb.h"
#include <QtGui>

class RuleCheckWidget		:public QDialog
{

public:
	RuleCheckWidget(QWidget* parent=NULL);
	void		setData(PBNS::OprationMsg_Response &res);

private:
	void		initUi();
	void		initData();
	void		initTable();
	void		initSlots();
private:
	QPushButton*			m_okBtn;
	QPushButton*			m_cancelBtn;
	QTableWidget*		m_saveList;
	PBNS::OprationMsg_Response m_dataList;
};

#endif