/************************************************************************/
/* 
	DESC:		规则编辑窗口
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
/*                         规则类型编辑管理                             */
/************************************************************************/
class RuleTypeMgrdlg : public QDialog
{
	Q_OBJECT

public:
	static RuleTypeMgrdlg* instance();
	RuleTypeMgrdlg();
	~RuleTypeMgrdlg();

	void reqRuleList();										    //请求规则列表
	void retRuleManager(int msgtype,const char* msg);			//规则管理

private:
	void setRulelevel();
	void retRuleList(const char* msg);							//得到返回规则列表

signals:

public slots:
	void recvdata(int msgtype,const char* msg,int msglength);	// 接收数据
	void getRuleItem(QTableWidgetItem* item);					//选中tab表中某一行响应函数
	void addRule();
	void delRule();
	void mdfRule();

private:

	QLabel		*pridlab,*plevlab,*prnamelab,*pdesclab;			//标签:规则ID,风险等级,规则名称,规则描述
	QSpinBox	*pridpbox;										//输入框:规则ID
	QLineEdit	*pridlied,*prnamelined,*pdescld;				//输入框:规则ID,规则名称,规则描述
	QComboBox	*plevcbx;										//风险等级

	QTableWidget *ptabwdgrlist;									//规则列表
	QPushButton	*pbntadd,*pbntdel,*pbntmdf;						//操作按钮:添加,删除,修改

	static RuleTypeMgrdlg *m_inst;

	bool		m_addflg,m_delflg;								//添加规则标志，删除规则标志
	int			m_addrid,m_delrid;								//添加规则ID，删除规则ID
	bool		baddflag;										//添加规则标志
	bool		bmdfflag;										//修改规则标志

};


/************************************************************************/
/*                           厂站规则关系管理                           */
/************************************************************************/
class RuleMgrdlg : public QDialog
{
	Q_OBJECT

public:
	RuleMgrdlg(QWidget *parent);
	~RuleMgrdlg();

	void reqStaionRuleList();										    //请求站点规则关联关系列表
	void retStationRuleManager(int msgtype,const char* msg);			//站点规则关联关系管理

private:
	void loadSationList();												//加载厂站列表到combox中
	void loadRuleTypeList();											//加载规则类型列表到combox中
	void retStaionRuleList(const char* msg);							//得到返回规则列表

signals:

public slots:
	void	checkStachbox();											//点击选择厂站chbox按钮
	void	checkRulechbox();											//点击选择规则chbox按钮
	void	ChangeStaCbox(int idx);										//下拉选择具体厂站
	void	ChangeRuleCbox(int idx);									//下拉选择具体规则
	void	checkStationRuleUse();										//选择按厂站启用
	void	checkStationRuleNoUse();									//选择按厂站禁用
	void	checkRuleUse();												//选择按规则启用
	void	checkRuleNoUse();											//选择按规则禁用

	void	cellChangeIsUse(int row, int col);							//选择是否启用

	void	checkAllstachbox();											//点击选择全站启用chbox按钮
	void	checkCommitchbox();											//点击选择提交chbox按钮
	void	commitStationRule();										//提交厂站规则关联关系列表中是否启用
	void	allStationRuleUse();										//按全站启用
	void	createRuleTypeMgrDlg();										//创建规则类型对话框

private:

	QCheckBox	*pstachbox,*prulechbox;							//厂站复选框，规则复选框
	QCheckBox	*pstaallchbox,*pcmitchbox;						//全站规则启用复选框，提交复选框
	QComboBox	*pstacbox,*prulecbox;							//厂站下拉框，规则下拉框

	QPushButton	*pbtnstause,*pbtnstanouse;						//操作按钮:按厂站启用，禁止
	QPushButton	*pbtnruleuse,*pbtnrulenouse;					//操作按钮:按规则启用，禁止

	QPushButton *pbtnruletype,*pbtnalluse,*pbtncommit;			//规则类型，全站规则启用，提交
	QTableWidget *ptabwdgsrlist;								//厂站规则关联关系列表

	vector<Station_S> m_stationvct;										//保存厂站容器
	vector<RuleType_S> m_ruletypevct;									//保存规则类型容器
	vector<StationRule_S> m_starulevct;							//保存厂站规则列表容器
	vector<StationRule_S> m_stationrvct;						//按厂站保存厂站规则列表容器
	vector<StationRule_S> m_rulestvct;							//按规则保存厂站规则列表容器
	vector<StationRule_S> m_strulemdfvct;						//厂站规则关联关系修改列表容器

	QString		m_strstacimid;									//选择当前厂站的cimid
	int			m_ruleid;										//选择当前的规则ID

	bool		bcellchageflag;									//元素变化标志
	
};

#endif // RULEMGRDLG_H
