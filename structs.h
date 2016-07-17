/************************************************************************/
/* 

	DESC:	定义图形相关数据结构. 
	DATE:	2016-03-24
	AUTHOR:	YUANLS
	
*/
/************************************************************************/

#ifndef __SVGSTRUCT_H__
#define __SVGSTRUCT_H__
#include <QString>
#include <QList>
#include <QMap>
#include <QDomDocument>

struct TreeNode
{
	QString			label;
	int				level;
	bool			collapse;
	bool			theLast;
	int				count;
	QString		nodeId;
	QString		filePath;
	std::list<TreeNode*> children;
	TreeNode()
	{
		label = "";
		level = 1;
		collapse = true;
		theLast = false;
		count = 0;
		nodeId ="";
		filePath = "";
	}
};

struct ListNode
{
	QString label;
	TreeNode* treeNode;
};

// FTP服务器信息

struct FtpConfig
{
	QString				m_ftpAddr;
	int					m_ftpPort;
	QString				m_ftpUser;
	QString				m_ftpPwd;
	QString				m_ftpDir;
};

//厂站类型
struct StationType_S
{
	int id;
	int odernum;
	QString name;
};

//厂站信息
struct Station_S
{
	int id;
	int stypeid;
	QString stypename;
	QString name;
	QString cimid;
	QString curname;
	QString path;
};

//规则类型
struct RuleType_S
{
	int ruleid;
	QString rulename;
	int	rulelevel;
	QString ruledescrip;
};

//厂站关联规则信息
struct StationRule_S
{
	QString stationCim;
	QString stationName;
	int ruleId;
	QString ruleName;
	QString isUse;
	QString isUseName;
};

//用户信息
struct UserInfo_S
{
	int userid;
	QString username;
	QString userpwd;
	int userrole;
	QString rolename;
	QString realname;

};

//操作任务
struct TicketMsion_S
{
	int id;
	int userid;
	QString username;
	int actuserid;
	QString actusername;
	QString name;
	QString publishtime;
};

// 操作票
struct Ticket_S
{
	int id;
	int CuserId;
	QString CuserName;
	int AuserId;
	QString AuserName;
	int MissionId;
	QString MissionName;
	QString No;
	QString ActionType;
	QString info;
	QString ActionPerson;
	QString ProtectPerson;
	QString ChargePerson;
	QString StartTime;
	QString EndTime;
	QString PublishTime;
};

struct TicketActions_S
{
	int Ticketid;
	int OrderNum;
	QString SystemContent;
	QString Content;
};

#endif