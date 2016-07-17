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

/************************************************************************/
/*                           ����Ʊ��ϸ                                 */
/************************************************************************/
class TicketActionsMgr	:public QDialog
{
	Q_OBJECT

public:
	TicketActionsMgr(QWidget* parent = NULL);

	//���ò�������
	void setTicketMsion(TicketMsion_S& tketMsion){m_tketMsion = tketMsion;}

	//���ò�������
	void setTicket(Ticket_S& tket){m_tket = tket;}

	//������ʾ����
	void setViewType(int nType){m_viewType = nType;}

	//��ȡ��������
	QString getActionsType();

	//��ȡ����Ʊ��Ϣ
	Ticket_S getTicketInfo();

	//��ʼ������
	void		initDataByTicketMsion();
	//��ʼ������
	void		initDataByTicket();
	//���ղ���Ʊ�б�
	void retTicketActionsList(const char* msg);

	//���ò�������
	void		setActionsType();

private slots:

private:
	//��������Ʊ����
	void		initUi_Operator();

	//�������Ʊ�б�
	void reqTicketActionsList();

	


private:

	QLineEdit*		m_companyLied;								//��λ����
	QLineEdit*		m_noLied;									//��λ���
	QLineEdit*		m_createLied;								//������
	QLineEdit*		m_actLied;									//������
	QLineEdit*		m_missionLied;								//��������
	QLineEdit*		m_remarksLied;								//��ע
	QLineEdit*		m_operatorLied;								//������
	QLineEdit*		m_guardianLied;								//�໤��
	QLineEdit*		m_watcherLied;								//ֵ�ฺ����

	QCheckBox*		m_guardianChbox;							//�໤�²���
	QCheckBox*		m_onerChbox;								//���˲���
	QCheckBox*		m_overhaulChbox;							//������Ա����

	QDateTimeEdit*	m_createrTime;								//��������Ʊʱ��
	QDateTimeEdit*	m_opstartTime;								//������ʼʱ��
	QDateTimeEdit*	m_opendTime;								//��������ʱ��

	QTableWidget*	m_ticketActTable;							//����Ʊ��ϸ�б�


	TicketMsion_S	m_tketMsion;								//��������
	Ticket_S		m_tket;										//����Ʊ
	Ticket_S		m_tketReturn;								//���صĲ���Ʊ
	int		m_Msionid;											//��������ID
	QString m_strGuandian;										//�໤�²���
	QString m_strOner;											//���˲���
	QString m_strOverhaul;										//������Ա����
	QString m_strActType;										//��������
	int		m_viewType;											//��ʾ����1��ʾ������2��ʾ��ѯ
	
};


/************************************************************************/
/*                           ����Ʊ                                     */
/************************************************************************/
class TicketMgr	:public QDialog
{
	Q_OBJECT

public:
	TicketMgr(QWidget* parent = NULL);

	//�������Ʊ�����б�
	void reqTicketMsionList();

	//���ղ���Ʊ�����б�
	void retTicketMsionList(const char* msg);

	//�������Ʊ�б�
	void reqTicketList();

	//���ղ���Ʊ�б�
	void retTicketList(const char* msg);

	// ��������
	void recvdata(int msgtype,const char* msg,int msglength);

	//

private slots:
	void getTicketMsionItem(QTableWidgetItem* item);					//ѡ��tab����ĳһ����Ӧ����
	void getTicketItem(QTableWidgetItem* item);							//ѡ�в���Ʊtab����ĳһ����Ӧ����
	// ��������Ʊ
	void		onCreateActions();
	//�ύ����Ʊ
	void		onCommitActions();
	//��ѯ����Ʊ
	void		onQueryActions();
	//����ɾ���޸�
	void		onMdf();
	void		onAdd();
	void		onDel();

private:
	// ����Ա�����������
	void		initUi_Mission();

	// ����Ա����
	void		initUi_Action();

	//��������Ʊ����
	void		initUi_Operator();

	void		initSlots();

	void		initData();

	//����ִ�����е�ֵ�����б�
	void setUserCombox();

private:
	QComboBox*		m_userBox;									//ִ�����û������б�
	QLineEdit*		m_nameLied;									//������������
	QTableWidget*	m_missionTable;								//���������б�
	QDateTimeEdit*	m_crateTime;								//��������Ʊʱ��
	QPushButton*		m_mdfBtn;								//�޸Ĳ�������
	QPushButton*		m_addBtn;								//���Ӳ�������
	QPushButton*		m_delBtu;								//ɾ����������

	QPushButton*		m_createBtn;							//��������Ʊ
	QPushButton*		m_comitBtn;								//�ύ����Ʊ
	QPushButton*		m_queryBtn;								//��ѯ����Ʊ

	QTableWidget*	m_ticketTable;								//����Ʊ�б�

	//�û��б�
	vector<UserInfo_S>	m_userList;
	TicketMsion_S	m_ticketMsion;								//��������
	Ticket_S		m_ticket;									//����Ʊ
	bool		baddflag;										//��ӱ�־
	bool		bmdfflag;										//�޸ı�־

	//����Ʊ��ϸ����
	TicketActionsMgr	*m_tktActmgr;
	bool				m_bcomitflag;							//�ύ��־
	bool				m_bdlgflag;								//�Ƿ��Ѿ���������Ʊ��ϸ�Ի���
	QString m_strActType;										//��������

};




#endif