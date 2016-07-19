/************************************************************************/
/* 
	DESC:		��վ����ͳ�վ���͹���
	DATE:		2016-05-21
	AUTHOR:		Abelsky
*/
/************************************************************************/
#ifndef STATIONMGR_H
#define STATIONMGR_H

#include <QObject>
#include <QtGui>
#include <QDialog>
#include <QWidget>
#include <QTabWidget>
#include <QPushButton>
#include <QTextEdit>
#include <QComboBox>
#include <QLineEdit>
#include <QSpinBox>
#include <QStringList>
#include "structs.h"
#include "comutil.h"
#include "netclient.h"

/************************************************************************/
/*                           վ�����͹���                               */
/************************************************************************/
class StationTypeMgr : public QDialog
{

	Q_OBJECT

public:
	static StationTypeMgr* instance();
	StationTypeMgr();
	~StationTypeMgr();

public slots:
	void	recvdata(int msgtype,const char* msg,int msglength);		// ��������
	void	getStationTypeItem(QTableWidgetItem *item);					//�õ���վ�����б���ITEM��Ӧֵ
	void	addStationType();											//��ӳ�վ����
	void	delStationType();											//ɾ����վ����
	void	mdfStationType();											//�޸ĳ�վ����

private:
	void		loadStationTypeList();									//���س�վ�����б�
	void		reqStationTypeList();									//����վ�����б�
	void		retStationTypeList(int msgtype,const char* msg);		//���ճ�վ�����б�

private:
	QTableWidget	*ptabwdgStaTypeList;								//վ���б�
	QLabel			*porderlab,*ptypelab;								//��ǩ:��վ˳��,��վ��������
	QLineEdit		*porderlined,*ptypelined;							//�����:��վ˳��,��վ��������
	QSpinBox		*porderspbox;										//�����:��վ˳��
	QPushButton		*pbntadd,*pbntdel,*pbntmdf;							//������ť:���,ɾ��,�޸�
	bool			bflgadd,bflgmdf;									//���ӣ��޸ı�־

	vector<StationType_S> m_stationtype;								//���泧վ��������

	static StationTypeMgr *m_inst;

};


/************************************************************************/
/*                          վ��༭������                              */
/************************************************************************/
class StationEditMgr : public QDialog
{
	Q_OBJECT

public:
	StationEditMgr(QWidget *parent);
	~StationEditMgr();

	//���õ�ǰѡ�е�վ��ֵ
	void		setStationSelectVal(Station_S& sta){m_station = sta;};
	Station_S	getStationSelectVal(){return m_station;};					//�õ�ѡ���޸ĳ�վ
	void		loadStationEditData();										//���س�վ�༭����
	bool		getConfirmStationFlag();									//�õ�ȷ�ϳ�վ��־

private:
	void		setStationTypeCbox();										//���ó�վ����
	void		setStationPathCbox();										//���ó�վsvg�ļ���������

public slots:
	void		confirmStation();											//ȷ���޸ĳ�վ


private:

	QComboBox		*pstatypecbox,*ppathcbox;								//��վ���ͣ���վ����svg�ļ���
	QLineEdit		*pidlined,*pcimidlined,*pnamelined;						//�����:ID,CIMDID,��վ���ƣ���վ��������վ����svg�ļ���
	QLineEdit		*pstadisclined,*ppathlined;								//�����:��վ��������վ����svg�ļ���
	QPushButton		*pbntok;												//ȷ�ϰ�ť

	Station_S		m_station;												//��վ�ṹ��

	vector<StationType_S> m_stationtype;									//���泧վ��������

	QList<QString>	m_svgflist;												//FTP��svg�����б�

	bool			bcfmflag;												//�޸�ȷ�ϱ�־

};



/************************************************************************/
/*                            վ�������                                */
/************************************************************************/
class StationMgr : public QDialog
{
	Q_OBJECT

public:
	StationMgr(QWidget *parent);
	~StationMgr();

	
	void		retStationMgr(int msgtype,const char* msg);				//��վ������
private:
	void		loadStationList();										//���س�վ�б�
	void		setStationStruct(QTableWidgetItem * item);				//���ó�վ�ṹ��ֵ

public slots:
	void	createStaTypeMgrDlg();										//������վ���ͱ༭�Ի���
	void	createStationMgrDlg(QTableWidgetItem * item);				//������վ�༭�Ի���
	void	commitStation();											//�ύ�޸ĳ�վ


private:
	QTableWidget	*ptabwdgStaList;									//վ���б�
	QPushButton		*pbntStaType,*pbnCommit;							//������ť:��վ����,ȷ���ύ
	vector<Station_S> m_stationvct;										//���泧վ����
	vector<Station_S> m_stationmdfvct;									//����޸ĳ�վ����
	Station_S		m_station;											//��վ

	StationEditMgr	*pStationEditMgrDlg;								//��վ����༭��ָ��

	StationTypeMgr	*pStaTypeMgrDlg;

};










#endif // STATIONMGR_H
