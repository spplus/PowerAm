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
QString ComUtil::getFtpAddr()
{
	return m_ftpAddr;
}

QString ComUtil::getFtpPort()
{
	return m_ftpPort;
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
		m_ftpAddr = ftpNode.toElement().attribute("addr");
		m_ftpPort = ftpNode.toElement().attribute("port");
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