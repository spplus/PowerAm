#include <QTextCodec>
#include <QCoreApplication>

#include "comutil.h"

ComUtil* ComUtil::m_inst = NULL;

ComUtil::ComUtil()
{
	m_config = new QSettings(QCoreApplication::applicationDirPath()+"/"+CONFIG,QSettings::IniFormat);
	m_config->setIniCodec(QTextCodec::codecForName("GB2312")); 
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
	//QSettings syncini(QCoreApplication::applicationDirPath()+"/client.conf",QSettings::IniFormat);
	//syncini.setIniCodec("UTF8");
	//syncini.setIniCodec(QTextCodec::codecForName("GB2312")); 

	return m_config->value("AM/SysName").toString();
}

QString ComUtil::getSvgRoot()
{
	//QSettings syncini(QCoreApplication::applicationDirPath()+"/client.conf",QSettings::IniFormat);
	//syncini.setIniCodec("UTF8");
	//syncini.setIniCodec(QTextCodec::codecForName("GB2312")); 

	return m_config->value("AM/SvgRoot").toString();
}

void ComUtil::getStationType()
{
	PBNS::StationTypeMsg_Request req;
	req.set_saveid(1);
	string reqstr;
	req.SerializeToString(&reqstr);
	NetClient::instance()->sendData(CMD_STATION_TYPE,reqstr.c_str(),reqstr.length());
}

vector<TreeNode*> ComUtil::getStationList()
{
	return m_stationList;
}

void ComUtil::saveStationList(PBNS::StationTypeMsg_Response& res)
{
	for (int i = 0;i<res.typelist_size();i++)
	{
		PBNS::StationTypeBean bean = res.typelist(i);
		TreeNode* pnode = new TreeNode;
		pnode->label = bean.name().c_str();
		pnode->count = bean.stationlist_size();
		pnode->level = 1;
		pnode->nodeId = bean.id();
		pnode->theLast = (i==res.typelist_size()-1?true:false);
		if (i == 0)
		{
			pnode->collapse = false;
		}
		for (int j = 0;j<bean.stationlist_size();j++)
		{
			PBNS::StationBean sbean = bean.stationlist(j);
			TreeNode *cnode = new TreeNode;
			cnode->label = sbean.currentname().c_str();
			cnode->filePath = sbean.path().c_str();
			cnode->level = 2;
			cnode->nodeId = sbean.id();
			cnode->theLast = (j==bean.stationlist_size()-1?true:false);

			pnode->children.push_back(cnode);
		}

		m_stationList.push_back(pnode);
	}
}