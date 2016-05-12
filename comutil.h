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
#include <vector>
#include "structs.h"

using namespace std;
class ComUtil
{

public:
	
	// 
	static ComUtil*		instance();

	//  ��ȡϵͳ����
	QString				getSysName();

	// ���ط����б�
	void						getStationType();

	// ����վ���б�
	void						saveStationList(PBNS::StationTypeMsg_Response& res);

	// ��ȡվ���б�
	std::vector<TreeNode*>	getStationList();
private:
	ComUtil();

private:
	static	ComUtil*	m_inst;

	vector<TreeNode*>	m_stationList;
};

#endif