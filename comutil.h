/************************************************************************/
/* 
		DESC:	公共方法类.
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

	//  获取系统名称
	QString				getSysName();

	// 加载分类列表
	void						getStationType();

	// 保存站点列表
	void						saveStationList(PBNS::StationTypeMsg_Response& res);

	// 获取站点列表
	std::vector<TreeNode*>	getStationList();
private:
	ComUtil();

private:
	static	ComUtil*	m_inst;

	vector<TreeNode*>	m_stationList;
};

#endif