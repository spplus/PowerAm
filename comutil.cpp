#include <QTextCodec>
#include <QCoreApplication>
#include <QSettings>
#include "comutil.h"

ComUtil* ComUtil::m_inst = NULL;

ComUtil::ComUtil()
{

}

ComUtil* ComUtil::instance()
{
	if (m_inst == NULL)
	{
		m_inst = new ComUtil;
	}
	return m_inst;
}

QString ComUtil::getSysName()
{
	QSettings syncini(QCoreApplication::applicationDirPath()+"/client.conf",QSettings::IniFormat);
	syncini.setIniCodec("UTF8");
	syncini.setIniCodec(QTextCodec::codecForName("GB2312")); 

	return syncini.value("AM/SysName").toString();
}

void ComUtil::getStationType()
{
	PBNS::StationTypeMsg_Request req;
	req.set_saveid(1);
	string reqstr;
	req.SerializeToString(&reqstr);
	NetClient::instance()->sendData(CMD_STATION_TYPE,reqstr.c_str(),reqstr.length());
}

void ComUtil::saveStationList(TreeNode* node,int nodeid)
{
	for (int i = 0;i<m_stationList.size();i++)
	{
		TreeNode* pnode = m_stationList.at(i);
		if (pnode->nodeId == nodeid)
		{
			pnode->children.push_back(node);
		}
	}
}

void ComUtil::saveStationType(TreeNode* node)
{
	m_stationList.push_back(node);
}

vector<TreeNode*> ComUtil::getStationList()
{
	return m_stationList;
}