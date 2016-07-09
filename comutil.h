/************************************************************************/
/* 
		DESC:	����������.
		DATE:	2016-05-10
		AUTHOR:	YUANLS	
*/
/************************************************************************/
#ifndef __COMUTIL_H__
#define __COMUTIL_H__

#include "buff/msgbody.pb.h"
#include "define.h"
#include "netclient.h"
#include "include/commands.h"
#include <QString>
#include <string>
#include <QSettings>
#include <vector>
#include "structs.h"
#include "ftputil.h"

using namespace std;
class ComUtil
{

public:
	
	// ��ȡ��ǰʵ������
	static ComUtil*		instance();

	// ���������ļ�
	bool					initConfig();
	
	// ��ȡ��ǰ����Ŀ¼
	QString				getAppPath();

	//  ��ȡϵͳ����
	QString				getSysName();

	// ��ȡ������IP
	QString				getSvrAddr();

	// ��ȡ�������˿�
	QString				getSvrPort();

	// ��ȡSVG�ļ�����Ŀ¼
	QString				getSvgRoot();

	FtpConfig			getFtpConfig();
	
	// ���ط����б�
	void				getStationType();

	// ����վ���б�
	void				saveStationList(PBNS::StationTypeMsg_Response& res);


	//���û�ȡftp·���µ�svg�ļ��б�
	void				saveSvgPathName();

	//��ȡftp·���µ�svg�ļ��б�
	QList<QString>		getSvgPathName();

	//���س�վ�б�
	void				getStation();
	//���洿��վ�б�
	void				saveStationListonly(PBNS::StationListMsg_Response& res);
	//���洿��վ�����б�
	void				saveStationTypeList(PBNS::StationTypeMsg_Response& res);

	//���ع��������б�
	void				getRuleType();
	//������������б�
	void				saveRuleTypeList(PBNS::RuleListMsg_Response& res);

	//��ȡ���������б�
	vector<RuleType_S> getRuleTypeList();

	//��ȡ��վ�����б�
	vector<StationType_S> getStationTypeMgrList();

	//��ȡ��վ�б�
	vector<Station_S>	getStationMgrList();

	void				setStationTypeRushflag(bool brush){m_bStatype = brush;};
	bool				getStationTypeRushflag(){return m_bStatype;};

	// ��ȡվ���б�
	vector<TreeNode*>	getStationList();

	// ������ɫ����
	bool				loadColorRule();

	// ��ȡ��ɫ����
	QMap<QString,QString>&	getStyleMap();


private:
	ComUtil();
	
	bool		openFile(QString fname,QDomDocument &doc);
private:
	// ��ǰ����Ŀ¼
	QString				m_appPath;

	// ��ǰ����ʵ��
	static	ComUtil*		m_inst;

	// FTP����
	//FtpUtil*				m_ftpUtil;

	// ��������ַ
	QString				m_svrAddr;
	QString				m_svrPort;
	QString				m_sysName;
	QString				m_svgRoot;
	FtpConfig			m_ftpConfig;

	// ��ɫ��������
	QMap<QString,QString>		m_styleMap;

	// վ���б�
	vector<TreeNode*>			m_stationList;


	//��վ�����б�
	vector<StationType_S>		m_stationtypelist;

	//��վ�б�
	vector<Station_S>			m_staList;

	//���������б�
	vector<RuleType_S>			m_rulelist;

	// �ļ��б�
	QList<QString>				m_svgflist;

	//���泧վˢ�±�־
	bool						m_bStatype;

};

#endif