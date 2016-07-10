/************************************************************************/
/* 
		DESC:	����Ʊ�������.
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

	//�������Ʊ�����б�
	void reqTicketMsionList();

	//���ղ���Ʊ�����б�
	void retTicketMsionList(const char* msg);

	// ��������
	void recvdata(int msgtype,const char* msg,int msglength);

private slots:
	void getTicketMsionItem(QTableWidgetItem* item);					//ѡ��tab����ĳһ����Ӧ����
	// ��������Ʊ
	void		onCreateActions();

	//����ɾ���޸�
	void		onMdf();
	void		onAdd();
	void		onDel();

private:
	// ����Ա�����������
	void		initUi_Mission();

	// ����Ա����
	void		initUi_Action();

	void		initSlots();

	void		initData();

	//����ִ�����е�ֵ�����б�
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

	//�û��б�
	vector<UserInfo_S>	m_userList;

	bool		baddflag;										//��ӱ�־
	bool		bmdfflag;										//�޸ı�־
};

#endif