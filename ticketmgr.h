/************************************************************************/
/* 
		DESC:	操作票任务管理.
		DATE:	2016-07-09
		AUTHOR:	YUANLS
*/
/************************************************************************/

#ifndef __TICKETMGR_H__
#define __TICKETMGR_H__
#include <QtGui>

class TicketMgr		:public QDialog
{
public:
	TicketMgr(QWidget* parent = NULL);

private slots:

	// 创建操作票
	void		onCreateActions();

	// 保存
	void		onSave();
	void		onAdd();
	void		onDel();

private:
	// 调度员创建命令界面
	void		initUi_Mission();

	// 操作员界面
	void		initUi_Action();

	void		initSlots();

	void		initData();

private:
	QComboBox*		m_userBox;
	QTableWidget*	m_ticketTable;
	QTableWidget*	m_missionTable;
	QDateTimeEdit*	m_crateTime;
	QPushButton*		m_saveBtn;
	QPushButton*		m_addBtn;
	QPushButton*		m_delBtu;
	QPushButton*		m_createBtn;
};

#endif