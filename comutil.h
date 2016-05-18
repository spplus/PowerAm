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
#include <QSettings>
#include <vector>
#include "structs.h"

using namespace std;
class ComUtil
{

public:
	
	// 获取当前实例对象
	static ComUtil*		instance();

	// 加载配置文件
	bool							initConfig();
	
	// 获取当前运行目录
	QString				getAppPath();

	//  获取系统名称
	QString				getSysName();

	// 获取服务器IP
	QString				getSvrAddr();

	// 获取服务器端口
	QString				getSvrPort();

	// 获取SVG文件保存目录
	QString				getSvgRoot();

	QString				getFtpAddr();
	QString				getFtpPort();

	// 加载分类列表
	void						getStationType();

	// 保存站点列表
	void						saveStationList(PBNS::StationTypeMsg_Response& res);

	// 获取站点列表
	vector<TreeNode*>		getStationList();

	// 加载颜色规则
	bool					loadColorRule();

	// 获取颜色规则
	QMap<QString,QString>&	getStyleMap();

private:
	ComUtil();
	
	bool		openFile(QString fname,QDomDocument &doc);
private:
	// 当前运行目录
	QString						m_appPath;

	// 当前对象实例
	static	ComUtil*			m_inst;

	// 服务器地址
	QString						m_svrAddr;
	QString						m_svrPort;
	QString						m_sysName;
	QString						m_svgRoot;
	QString						m_ftpAddr;
	QString						m_ftpPort;

	// 颜色规则隐射
	QMap<QString,QString>		m_styleMap;

	// 站点列表
	vector<TreeNode*>			m_stationList;
};

#endif