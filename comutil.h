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
#include <QVector>
#include "structs.h"
#include "ftputil.h"

using namespace std;
class ComUtil
{

public:
	
	// 获取当前实例对象
	static ComUtil*		instance();

	// 加载配置文件
	bool					initConfig();
	
	// 获取当前运行目录
	QString				getAppPath();

	//  获取系统名称
	QString				getSysName();

	// 获取服务器IP
	QString				getSvrAddr();

	// 获取服务器端口
	QString				getSvrPort();

	//获取公司名称
	QString				getCompanyName();

	// 获取SVG文件保存目录
	QString				getSvgRoot();

	FtpConfig			getFtpConfig();
	
	// 加载分类列表
	void				getStationType();

	// 保存站点列表
	void				saveStationList(PBNS::StationTypeMsg_Response& res);


	//设置获取ftp路径下的svg文件列表
	void				saveSvgPathName();

	//获取ftp路径下的svg文件列表
	QList<QString>		getSvgPathName();

	//加载厂站列表
	void				getStation();
	//保存纯厂站列表
	void				saveStationListonly(PBNS::StationListMsg_Response& res);
	//保存纯厂站类型列表
	void				saveStationTypeList(PBNS::StationTypeMsg_Response& res);

	//加载规则类型列表
	void				getRuleType();
	//保存规则类型列表
	void				saveRuleTypeList(PBNS::RuleListMsg_Response& res);

	//获取规则类型列表
	vector<RuleType_S> getRuleTypeList();

	//获取厂站类型列表
	vector<StationType_S> getStationTypeMgrList();

	//获取厂站列表
	vector<Station_S>	getStationMgrList();

	void				setStationTypeRushflag(bool brush){m_bStatype = brush;};
	bool				getStationTypeRushflag(){return m_bStatype;};

	// 获取站点列表
	QVector<TreeNode*>	getStationList();

	// 加载颜色规则
	bool				loadColorRule();

	// 获取颜色规则
	QMap<QString,QString>&	getStyleMap();

	// 获取当前用户的角色
	int				getCurUserRole();
	void			setCurUserRole(int roleid);

	// 获取当前用户ID
	int				getCurUserId();
	void			setCurUserId(int userid);

	//获取用户列表
	void			getAllUserList();
	//保存用户列表
	void			saveAllUserList(PBNS::UserListMsg_Response& res);

	//当用户编辑时更新用户列表
	void			setUserList(vector<UserInfo_S> vctUlist){m_userlist = vctUlist;};
	//取用户列表信息
	vector<UserInfo_S>	getUserList(){return m_userlist;};

	QString		now();
private:
	ComUtil();
	
	bool		openFile(QString fname,QDomDocument &doc);
private:

	int					m_curUserId;
	int					m_curUserRole;

	// 当前运行目录
	QString				m_appPath;

	// 当前对象实例
	static	ComUtil*		m_inst;

	// 服务器地址
	QString				m_svrAddr;
	QString				m_svrPort;
	QString				m_sysName;
	QString				m_companyName;
	QString				m_svgRoot;
	FtpConfig			m_ftpConfig;

	// 颜色规则隐射
	QMap<QString,QString>		m_styleMap;

	// 站点列表
	QVector<TreeNode*>			m_stationList;


	//厂站类型列表
	vector<StationType_S>		m_stationtypelist;

	//厂站列表
	vector<Station_S>			m_staList;

	//规则类型列表
	vector<RuleType_S>			m_rulelist;

	// 文件列表
	QList<QString>				m_svgflist;

	//保存厂站刷新标志
	bool						m_bStatype;

	//用户列表
	vector<UserInfo_S>			m_userlist;

};

#endif