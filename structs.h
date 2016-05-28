/************************************************************************/
/* 

	DESC:	����ͼ��������ݽṹ. 
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

// FTP��������Ϣ

struct FtpConfig
{
	QString				m_ftpAddr;
	int					m_ftpPort;
	QString				m_ftpUser;
	QString				m_ftpPwd;
	QString				m_ftpDir;
};

#endif