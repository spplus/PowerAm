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
#include <vector>
#include "comutil.h"
#include "structs.h"

class TicketMgr	:public QDialog
{
	Q_OBJECT

public:
	TicketMgr(QWidget* parent = NULL);

	//请求操作票任务列表
	void reqTicketMsionList();

	//接收操作票任务列表
	void retTicketMsionList(const char* msg);

	// 接收数据
	void recvdata(int msgtype,const char* msg,int msglength);

private slots:
	void getTicketMsionItem(QTableWidgetItem* item);					//选中tab表中某一行响应函数
	// 创建操作票
	void		onCreateActions();

	//增加删除修改
	void		onMdf();
	void		onAdd();
	void		onDel();

private:
	// 调度员创建命令界面
	void		initUi_Mission();

	// 操作员界面
	void		initUi_Action();

	void		initSlots();

	void		initData();

	//设置执行人中的值下拉列表
	void setUserCombox();

private:
	QComboBox*		m_userBox;
	QLineEdit*		m_nameLied;
	QTableWidget*	m_ticketTable;
	QTableWidget*	m_missionTable;
	QDateTimeEdit*	m_crateTime;
	QPushButton*		m_mdfBtn;
	QPushButton*		m_addBtn;
	QPushButton*		m_delBtu;
	QPushButton*		m_createBtn;

	//用户列表
	vector<UserInfo_S>	m_userList;

	bool		baddflag;										//添加标志
	bool		bmdfflag;										//修改标志
};

#endif