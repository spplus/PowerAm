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

using namespace std;
class ComUtil
{

public:
	
	// ��ȡ��ǰʵ������
	static ComUtil*		instance();
	
	// ��ȡ��ǰ����Ŀ¼
	QString				getAppPath();

	//  ��ȡϵͳ����
	QString				getSysName();

	// ��ȡSVG�ļ�����Ŀ¼
	QString				getSvgRoot();

	// ���ط����б�
	void						getStationType();

	// ����վ���б�
	void						saveStationList(PBNS::StationTypeMsg_Response& res);

	// ��ȡվ���б�
	vector<TreeNode*>		getStationList();

	// ������ɫ����
	bool					loadColorRule();

	// ��ȡ��ɫ����
	QMap<QString,QString>&	getStyleMap();

private:
	ComUtil();

private:
	// ��ǰ����Ŀ¼
	QString						m_appPath;

	// ��ǰ����ʵ��
	static	ComUtil*			m_inst;

	// �����ļ���ȡ����
	QSettings*					m_config;

	// ��ɫ��������
	QMap<QString,QString>		m_styleMap;

	// վ���б�
	vector<TreeNode*>			m_stationList;
};

#endif