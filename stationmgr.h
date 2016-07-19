/************************************************************************/
/* 
	DESC:		厂站管理和厂站类型管理
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
/*                           站点类型管理                               */
/************************************************************************/
class StationTypeMgr : public QDialog
{

	Q_OBJECT

public:
	static StationTypeMgr* instance();
	StationTypeMgr();
	~StationTypeMgr();

public slots:
	void	recvdata(int msgtype,const char* msg,int msglength);		// 接收数据
	void	getStationTypeItem(QTableWidgetItem *item);					//得到厂站类型列表中ITEM对应值
	void	addStationType();											//添加厂站类型
	void	delStationType();											//删除厂站类型
	void	mdfStationType();											//修改厂站类型

private:
	void		loadStationTypeList();									//加载厂站类型列表
	void		reqStationTypeList();									//请求厂站类型列表
	void		retStationTypeList(int msgtype,const char* msg);		//接收厂站类型列表

private:
	QTableWidget	*ptabwdgStaTypeList;								//站点列表
	QLabel			*porderlab,*ptypelab;								//标签:厂站顺序,厂站类型名称
	QLineEdit		*porderlined,*ptypelined;							//输入框:厂站顺序,厂站类型名称
	QSpinBox		*porderspbox;										//输入框:厂站顺序
	QPushButton		*pbntadd,*pbntdel,*pbntmdf;							//操作按钮:添加,删除,修改
	bool			bflgadd,bflgmdf;									//增加，修改标志

	vector<StationType_S> m_stationtype;								//保存厂站类型容器

	static StationTypeMgr *m_inst;

};


/************************************************************************/
/*                          站点编辑管理类                              */
/************************************************************************/
class StationEditMgr : public QDialog
{
	Q_OBJECT

public:
	StationEditMgr(QWidget *parent);
	~StationEditMgr();

	//设置当前选中的站点值
	void		setStationSelectVal(Station_S& sta){m_station = sta;};
	Station_S	getStationSelectVal(){return m_station;};					//得到选择修改厂站
	void		loadStationEditData();										//加载厂站编辑数据
	bool		getConfirmStationFlag();									//得到确认厂站标志

private:
	void		setStationTypeCbox();										//设置厂站类型
	void		setStationPathCbox();										//设置厂站svg文件名称类型

public slots:
	void		confirmStation();											//确认修改厂站


private:

	QComboBox		*pstatypecbox,*ppathcbox;								//厂站类型，厂站对于svg文件名
	QLineEdit		*pidlined,*pcimidlined,*pnamelined;						//输入框:ID,CIMDID,厂站名称，厂站描述，厂站对于svg文件名
	QLineEdit		*pstadisclined,*ppathlined;								//输入框:厂站描述，厂站对于svg文件名
	QPushButton		*pbntok;												//确认按钮

	Station_S		m_station;												//厂站结构体

	vector<StationType_S> m_stationtype;									//保存厂站类型容器

	QList<QString>	m_svgflist;												//FTP中svg名称列表

	bool			bcfmflag;												//修改确认标志

};



/************************************************************************/
/*                            站点管理类                                */
/************************************************************************/
class StationMgr : public QDialog
{
	Q_OBJECT

public:
	StationMgr(QWidget *parent);
	~StationMgr();

	
	void		retStationMgr(int msgtype,const char* msg);				//厂站管理返回
private:
	void		loadStationList();										//加载厂站列表
	void		setStationStruct(QTableWidgetItem * item);				//设置厂站结构体值

public slots:
	void	createStaTypeMgrDlg();										//创建厂站类型编辑对话框
	void	createStationMgrDlg(QTableWidgetItem * item);				//创建厂站编辑对话框
	void	commitStation();											//提交修改厂站


private:
	QTableWidget	*ptabwdgStaList;									//站点列表
	QPushButton		*pbntStaType,*pbnCommit;							//操作按钮:厂站类型,确认提交
	vector<Station_S> m_stationvct;										//保存厂站容器
	vector<Station_S> m_stationmdfvct;									//存放修改厂站容器
	Station_S		m_station;											//厂站

	StationEditMgr	*pStationEditMgrDlg;								//厂站管理编辑类指针

	StationTypeMgr	*pStaTypeMgrDlg;

};










#endif // STATIONMGR_H
