/************************************************************************/
/* 
	DESC:		����༭����
	DATE:		2016-06-18
	AUTHOR:		Abelsky
*/
/************************************************************************/

#ifndef RULEMGRDLG_H
#define RULEMGRDLG_H

#include <QObject>
#include <QtGui>
#include <QDialog>
#include <QWidget>
#include <QTabWidget>
#include <QPushButton>
#include <QTextEdit>
#include <QComboBox>
#include <QCheckBox>
#include <QLineEdit>
#include <QStringList>
#include "comutil.h"

#include "netclient.h"
#include "..\common\include\commands.h"
#include "..\common\buff\msgbody.pb.h"

#define	PBNS com::spplus::buff

/************************************************************************/
/*                         �������ͱ༭����                             */
/************************************************************************/
class RuleTypeMgrdlg : public QDialog
{
	Q_OBJECT

public:
	static RuleTypeMgrdlg* instance();
	RuleTypeMgrdlg();
	~RuleTypeMgrdlg();

	void reqRuleList();										    //��������б�
	void retRuleManager(int msgtype,const char* msg);			//�������

private:
	void setRulelevel();
	void retRuleList(const char* msg);							//�õ����ع����б�

signals:

public slots:
	void recvdata(int msgtype,const char* msg,int msglength);	// ��������
	void getRuleItem(QTableWidgetItem* item);					//ѡ��tab����ĳһ����Ӧ����
	void addRule();
	void delRule();
	void mdfRule();

private:

	QLabel		*pridlab,*plevlab,*prnamelab,*pdesclab;			//��ǩ:����ID,���յȼ�,��������,��������
	QSpinBox	*pridpbox;										//�����:����ID
	QLineEdit	*pridlied,*prnamelined,*pdescld;				//�����:����ID,��������,��������
	QComboBox	*plevcbx;										//���յȼ�

	QTableWidget *ptabwdgrlist;									//�����б�
	QPushButton	*pbntadd,*pbntdel,*pbntmdf;						//������ť:���,ɾ��,�޸�

	static RuleTypeMgrdlg *m_inst;

	bool		m_addflg,m_delflg;								//��ӹ����־��ɾ�������־
	int			m_addrid,m_delrid;								//��ӹ���ID��ɾ������ID
	bool		baddflag;										//��ӹ����־
	bool		bmdfflag;										//�޸Ĺ����־

};


/************************************************************************/
/*                           ��վ�����ϵ����                           */
/************************************************************************/
class RuleMgrdlg : public QDialog
{
	Q_OBJECT

public:
	RuleMgrdlg(QWidget *parent);
	~RuleMgrdlg();

	void reqStaionRuleList();										    //����վ����������ϵ�б�
	void retStationRuleManager(int msgtype,const char* msg);			//վ����������ϵ����

private:
	void loadSationList();												//���س�վ�б�combox��
	void loadRuleTypeList();											//���ع��������б�combox��
	void retStaionRuleList(const char* msg);							//�õ����ع����б�

signals:

public slots:
	void	checkStachbox();											//���ѡ��վchbox��ť
	void	checkRulechbox();											//���ѡ�����chbox��ť
	void	ChangeStaCbox(int idx);										//����ѡ����峧վ
	void	ChangeRuleCbox(int idx);									//����ѡ��������
	void	checkStationRuleUse();										//ѡ�񰴳�վ����
	void	checkStationRuleNoUse();									//ѡ�񰴳�վ����
	void	checkRuleUse();												//ѡ�񰴹�������
	void	checkRuleNoUse();											//ѡ�񰴹������

	void	cellChangeIsUse(int row, int col);							//ѡ���Ƿ�����

	void	checkAllstachbox();											//���ѡ��ȫվ����chbox��ť
	void	checkCommitchbox();											//���ѡ���ύchbox��ť
	void	commitStationRule();										//�ύ��վ���������ϵ�б����Ƿ�����
	void	allStationRuleUse();										//��ȫվ����
	void	createRuleTypeMgrDlg();										//�����������ͶԻ���

private:

	QCheckBox	*pstachbox,*prulechbox;							//��վ��ѡ�򣬹���ѡ��
	QCheckBox	*pstaallchbox,*pcmitchbox;						//ȫվ�������ø�ѡ���ύ��ѡ��
	QComboBox	*pstacbox,*prulecbox;							//��վ�����򣬹���������

	QPushButton	*pbtnstause,*pbtnstanouse;						//������ť:����վ���ã���ֹ
	QPushButton	*pbtnruleuse,*pbtnrulenouse;					//������ť:���������ã���ֹ

	QPushButton *pbtnruletype,*pbtnalluse,*pbtncommit;			//�������ͣ�ȫվ�������ã��ύ
	QTableWidget *ptabwdgsrlist;								//��վ���������ϵ�б�

	vector<Station_S> m_stationvct;										//���泧վ����
	vector<RuleType_S> m_ruletypevct;									//���������������
	vector<StationRule_S> m_starulevct;							//���泧վ�����б�����
	vector<StationRule_S> m_stationrvct;						//����վ���泧վ�����б�����
	vector<StationRule_S> m_rulestvct;							//�����򱣴泧վ�����б�����
	vector<StationRule_S> m_strulemdfvct;						//��վ���������ϵ�޸��б�����

	QString		m_strstacimid;									//ѡ��ǰ��վ��cimid
	int			m_ruleid;										//ѡ��ǰ�Ĺ���ID

	bool		bcellchageflag;									//Ԫ�ر仯��־
	
};

#endif // RULEMGRDLG_H
