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

/************************************************************************/
/*                           操作票明细                                 */
/************************************************************************/
class TicketActionsMgr	:public QDialog
{
	Q_OBJECT

public:
	TicketActionsMgr(QWidget* parent = NULL);

	//设置操作任务
	void setTicketMsion(TicketMsion_S& tketMsion){m_tketMsion = tketMsion;}

	//设置操作任务
	void setTicket(Ticket_S& tket){m_tket = tket;}

	//设置显示类型
	void setViewType(int nType){m_viewType = nType;}

	//获取操作类型
	QString getActionsType();

	//获取操作票信息
	Ticket_S getTicketInfo();

	//初始化数据
	void		initDataByTicketMsion();
	//初始化数据
	void		initDataByTicket();
	//接收操作票列表
	void retTicketActionsList(const char* msg);

	//设置操作类型
	void		setActionsType();

private slots:

private:
	//创建操作票界面
	void		initUi_Operator();

	//请求操作票列表
	void reqTicketActionsList();

	


private:

	QLineEdit*		m_companyLied;								//单位名称
	QLineEdit*		m_noLied;									//单位编号
	QLineEdit*		m_createLied;								//发令人
	QLineEdit*		m_actLied;									//受令人
	QLineEdit*		m_missionLied;								//操作任务
	QLineEdit*		m_remarksLied;								//备注
	QLineEdit*		m_operatorLied;								//操作人
	QLineEdit*		m_guardianLied;								//监护人
	QLineEdit*		m_watcherLied;								//值班负责人

	QCheckBox*		m_guardianChbox;							//监护下操作
	QCheckBox*		m_onerChbox;								//单人操作
	QCheckBox*		m_overhaulChbox;							//检修人员操作

	QDateTimeEdit*	m_createrTime;								//创建操作票时间
	QDateTimeEdit*	m_opstartTime;								//操作开始时间
	QDateTimeEdit*	m_opendTime;								//操作结束时间

	QTableWidget*	m_ticketActTable;							//操作票明细列表


	TicketMsion_S	m_tketMsion;								//操作任务
	Ticket_S		m_tket;										//操作票
	Ticket_S		m_tketReturn;								//返回的操作票
	int		m_Msionid;											//操作任务ID
	QString m_strGuandian;										//监护下操作
	QString m_strOner;											//单人操作
	QString m_strOverhaul;										//检修人员操作
	QString m_strActType;										//操作类型
	int		m_viewType;											//显示类型1表示创建，2表示查询
	
};


/************************************************************************/
/*                           操作票                                     */
/************************************************************************/
class TicketMgr	:public QDialog
{
	Q_OBJECT

public:
	TicketMgr(QWidget* parent = NULL);

	//请求操作票任务列表
	void reqTicketMsionList();

	//接收操作票任务列表
	void retTicketMsionList(const char* msg);

	//请求操作票列表
	void reqTicketList();

	//接收操作票列表
	void retTicketList(const char* msg);

	// 接收数据
	void recvdata(int msgtype,const char* msg,int msglength);

	//

private slots:
	void getTicketMsionItem(QTableWidgetItem* item);					//选中tab表中某一行响应函数
	void getTicketItem(QTableWidgetItem* item);							//选中操作票tab表中某一行响应函数
	// 创建操作票
	void		onCreateActions();
	//提交操作票
	void		onCommitActions();
	//查询操作票
	void		onQueryActions();
	//增加删除修改
	void		onMdf();
	void		onAdd();
	void		onDel();

private:
	// 调度员创建命令界面
	void		initUi_Mission();

	// 操作员界面
	void		initUi_Action();

	//创建操作票界面
	void		initUi_Operator();

	void		initSlots();

	void		initData();

	//设置执行人中的值下拉列表
	void setUserCombox();

private:
	QComboBox*		m_userBox;									//执行人用户下拉列表
	QLineEdit*		m_nameLied;									//操作任务名称
	QTableWidget*	m_missionTable;								//操作任务列表
	QDateTimeEdit*	m_crateTime;								//创建操作票时间
	QPushButton*		m_mdfBtn;								//修改操作任务
	QPushButton*		m_addBtn;								//增加操作任务
	QPushButton*		m_delBtu;								//删除操作任务

	QPushButton*		m_createBtn;							//创建操作票
	QPushButton*		m_comitBtn;								//提交操作票
	QPushButton*		m_queryBtn;								//查询操作票

	QTableWidget*	m_ticketTable;								//操作票列表

	//用户列表
	vector<UserInfo_S>	m_userList;
	TicketMsion_S	m_ticketMsion;								//操作任务
	Ticket_S		m_ticket;									//操作票
	bool		baddflag;										//添加标志
	bool		bmdfflag;										//修改标志

	//操作票明细对象
	TicketActionsMgr	*m_tktActmgr;
	bool				m_bcomitflag;							//提交标志
	bool				m_bdlgflag;								//是否已经创建操作票明细对话框
	QString m_strActType;										//操作类型

};




#endif