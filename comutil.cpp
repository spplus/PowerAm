#include <QTextCodec>
#include <QCoreApplication>
#include <QFile>
#include <QMessageBox>
#include <QDomDocument>

#include "comutil.h"

ComUtil* ComUtil::m_inst = NULL;

ComUtil::ComUtil()
{
	m_appPath = QCoreApplication::applicationDirPath();
}

ComUtil* ComUtil::instance()
{
	if (m_inst == NULL)
	{
		m_inst = new ComUtil;
	}

	return m_inst;
}
FtpConfig ComUtil::getFtpConfig()
{
	return m_ftpConfig;
}

QString ComUtil::getSysName()
{
	return m_sysName;
}

QString ComUtil::getSvrAddr()
{
	return m_svrAddr;
}

QString ComUtil::getSvrPort()
{
	return m_svrPort;
}

void ComUtil::setCurUserRole(int roleid)
{
	m_curUserRole = roleid;
}
int ComUtil::getCurUserRole()
{
	return m_curUserRole;
}

void ComUtil::setCurUserId(int userid)
{
	m_curUserId = userid;
}

int ComUtil::getCurUserId()
{
	return m_curUserId;
}

QString ComUtil::getCompanyName()
{
	return m_companyName;
}

QString ComUtil::getSvgRoot()
{
	return m_svgRoot;
}

void ComUtil::getStationType()
{
	PBNS::StationTypeMsg_Request req;
	req.set_saveid(1);
	string reqstr;
	req.SerializeToString(&reqstr);
	NetClient::instance()->sendData(CMD_STATION_TYPE,reqstr.c_str(),reqstr.length());
}

void ComUtil::getStation()
{
	PBNS::StationListMsg_Request req;
	req.set_stationid(1);
	string reqstr;
	req.SerializeToString(&reqstr);
	NetClient::instance()->sendData(CMD_STATION_LIST,reqstr.c_str(),reqstr.length());
}

void ComUtil::getRuleType()
{
	PBNS::RuleListMsg_Request req;
	req.set_reqdate("1");
	string reqstr;
	req.SerializeToString(&reqstr);
	NetClient::instance()->sendData(CMD_COM_RULE_LIST,reqstr.c_str(),reqstr.length());
}

void ComUtil::getAllUserList()
{
	PBNS::UserListMsg_Request ulreq;

	QString strcurUserid = QString("%1").arg(m_curUserId);

	ulreq.set_reqdate(strcurUserid.toStdString().c_str());

	//发射发送数据请求消息信号
	NetClient::instance()->sendData(CMD_ROLE_USER_LIST,ulreq.SerializeAsString().c_str(),ulreq.SerializeAsString().length());

}

void ComUtil::saveAllUserList(PBNS::UserListMsg_Response& res)
{
	m_userlist.clear();

	for (int i = 0;i<res.userlist_size();i++)
	{
		PBNS::UserBean ubean = res.userlist(i);
		UserInfo_S uinfo;

		uinfo.userid = atoi(ubean.userid().c_str());
		uinfo.username = ubean.username().c_str();
		uinfo.userrole = atoi(ubean.userrole().c_str());
		uinfo.rolename = ubean.rolename().c_str();
		uinfo.userpwd = ubean.userpwd().c_str();
		uinfo.realname = ubean.realname().c_str();

		m_userlist.push_back(uinfo);
	}
}

vector<TreeNode*> ComUtil::getStationList()
{
	return m_stationList;
}


vector<StationType_S> ComUtil::getStationTypeMgrList()
{
	return m_stationtypelist;
}


vector<Station_S> ComUtil::getStationMgrList()
{
	return m_staList;
}

vector<RuleType_S> ComUtil::getRuleTypeList()
{
	return m_rulelist;
}

QList<QString> ComUtil::getSvgPathName()
{
	return m_svgflist;
}

void ComUtil::saveStationList(PBNS::StationTypeMsg_Response& res)
{
	m_stationList.clear();

	for (int i = 0;i<res.typelist_size();i++)
	{
		PBNS::StationTypeBean bean = res.typelist(i);
		TreeNode* pnode = new TreeNode;
		pnode->label = bean.name().c_str();
		pnode->count = bean.stationlist_size();
		pnode->level = 1;
		//pnode->nodeId = bean.id();
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
			cnode->nodeId = sbean.cimid().c_str();
			cnode->theLast = (j==bean.stationlist_size()-1?true:false);

			pnode->children.push_back(cnode);
		}

		m_stationList.push_back(pnode);
	}
}

void ComUtil::saveStationListonly(PBNS::StationListMsg_Response& res)
{
	m_staList.clear();

	for (int i=0;i<res.stationlist_size();i++)
	{
		PBNS::StationBean stabean = res.stationlist(i);
		Station_S station;
		station.id = stabean.id();
		station.stypeid = stabean.categoryid();
		station.stypename = QString("");
		vector<StationType_S>::iterator iter;
		for (iter=m_stationtypelist.begin();iter!=m_stationtypelist.end();iter++)
		{
			if (iter->id == station.stypeid)
			{
				station.stypename = iter->name;
			}
		}
		station.cimid = stabean.cimid().c_str();
		station.name = stabean.name().c_str();
		station.curname = stabean.currentname().c_str();
		station.path = stabean.path().c_str();

		m_staList.push_back(station);
	}
}

void ComUtil::saveStationTypeList(PBNS::StationTypeMsg_Response& res)
{
	//m_staList.clear();
	m_stationtypelist.clear();

	for (int i=0;i<res.typelist_size();i++)
	{
		PBNS::StationTypeBean statypebean = res.typelist(i);
		StationType_S statype;
		statype.id = statypebean.id();
		statype.name = statypebean.name().c_str();
		statype.odernum = statypebean.ordernum();
		/*
		for(int j=0;j<statypebean.stationlist_size();j++)
		{
			PBNS::StationBean stabean = statypebean.stationlist(j);
			Station_S station;
			station.id = stabean.id();
			station.stypeid = stabean.categoryid();
			station.stypename = statypebean.name().c_str();
			station.cimid = stabean.cimid().c_str();
			station.name = stabean.name().c_str();
			station.curname = stabean.currentname().c_str();
			station.path = stabean.path().c_str();

			m_staList.push_back(station);
		}
		*/
		m_stationtypelist.push_back(statype);
	}

}

void ComUtil::saveRuleTypeList(PBNS::RuleListMsg_Response& res)
{
	m_rulelist.clear();

	for (int i=0;i<res.rulelist_size();i++)
	{
		PBNS::RuleBean rbean = res.rulelist(i);
		RuleType_S ruletype;
		ruletype.ruleid = rbean.id();
		ruletype.rulename = rbean.name().c_str();
		ruletype.rulelevel = atoi(rbean.alarmlevel().c_str());
		ruletype.ruledescrip = rbean.description().c_str();

		m_rulelist.push_back(ruletype);
	}

}

void ComUtil::saveSvgPathName()
{
	//FtpUtil ftputil;
	//m_svgflist = ftputil.getFileList();
	m_svgflist = FtpUtil::instance()->getFileList();
}

bool ComUtil::openFile(QString fname,QDomDocument &doc)
{
	QFile *xmlFile;
	xmlFile = new QFile(m_appPath+"/"+fname);

	if (!xmlFile->open(QIODevice::ReadOnly)){

		QString title = QString("打开%1文件").arg(fname);
		QString text  = QString("打开%1文件,可能不存在该文件!").arg(xmlFile->fileName());
		QMessageBox::warning(NULL,title,text);
		return false;
	}

	if (!doc.setContent(xmlFile)) {

		QString title = QString("装载%1文件").arg(fname);
		QString text  = QString("装载%1文件,可能该文件有错误!").arg(xmlFile->fileName());
		QMessageBox::warning(NULL,title,text);

		xmlFile->close();
		return false;
	}
	xmlFile->close();
	return true;
}

bool ComUtil::loadColorRule()
{
	QDomDocument doc;
	if (!openFile(COLOR,doc))
	{
		return false;
	}

	// 解析颜色规则
	QDomNodeList rootNodes = doc.elementsByTagName("voltagedefine");
	if (rootNodes.count()>0)
	{
		QDomNode rootNode = rootNodes.at(0);
		QDomNodeList childNodes = rootNode.childNodes();
		for (int i = 0;i<childNodes.count();i++)
		{
			QDomNode cnode = childNodes.at(i);
			QString key = cnode.toElement().attribute("voltagename");
			QString val = cnode.toElement().attribute("color");
			m_styleMap.insert(key,val);
		}
	}
	return true;
}

bool ComUtil::initConfig()
{
	QDomDocument doc;
	if (!openFile(CLIENT,doc))
	{
		return false;
	}

	// 服务器配置
	QDomNodeList svrNodes = doc.elementsByTagName("server");
	if (svrNodes.count()>0)
	{
		QDomNode svrNode = svrNodes.at(0);
		m_svrAddr = svrNode.toElement().attribute("addr");
		m_svrPort = svrNode.toElement().attribute("port");

	}
	else
	{
		return false;
	}
	
	// FTP 配置
	QDomNodeList ftpNodes = doc.elementsByTagName("ftp");
	if (ftpNodes.count()>0)
	{
		QDomNode ftpNode = ftpNodes.at(0);
		m_ftpConfig.m_ftpAddr = ftpNode.toElement().attribute("addr");
		m_ftpConfig.m_ftpPort = ftpNode.toElement().attribute("port").toInt();
		m_ftpConfig.m_ftpUser = ftpNode.toElement().attribute("username");
		m_ftpConfig.m_ftpPwd = ftpNode.toElement().attribute("pwd");
		m_ftpConfig.m_ftpDir = ftpNode.toElement().attribute("dir");

	}
	else
	{
		return false;
	}

	// 系统名称
	QDomNodeList titleNodes = doc.elementsByTagName("title");
	if (titleNodes.count()>0)
	{
		QDomNode titleNode = titleNodes.at(0);
		m_sysName = titleNode.toElement().attribute("name");
	}
	else
	{
		return false;
	}

	// 公司名称
	QDomNodeList companyNodes = doc.elementsByTagName("company");
	if (companyNodes.count()>0)
	{
		QDomNode companyNode = companyNodes.at(0);
		m_companyName = companyNode.toElement().attribute("name");
	}
	else
	{
		return false;
	}

	// svg文件目录
	QDomNodeList svgNodes = doc.elementsByTagName("svgroot");
	if (svgNodes.count()>0)
	{
		QDomNode svgNode = svgNodes.at(0);
		m_svgRoot = svgNode.toElement().attribute("path");
	}
	else
	{
		return false;
	}
	return true;
}

QMap<QString,QString>& ComUtil::getStyleMap()
{
	return m_styleMap;
}

QString ComUtil::getAppPath()
{
	return m_appPath;
}

QString ComUtil::now()
{
	return QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz"); 
}