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

struct StationType_S
{
	int id;
	int odernum;
	QString name;
};

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

struct RuleType_S
{
	int ruleid;
	QString rulename;
	int	rulelevel;
	QString ruledescrip;
};

struct StationRule_S
{
	QString stationCim;
	QString stationName;
	int ruleId;
	QString ruleName;
	QString isUse;
	QString isUseName;
};

struct UserInfo_S
{
	int userid;
	QString username;
	QString userpwd;
	int userrole;
	QString rolename;
	QString realname;

};

#endif