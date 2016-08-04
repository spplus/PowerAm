#include "svggraph.h"

SvgGraph::SvgGraph()
{
	m_id = "";
	m_filePath = "";
	m_dom = new QDomDocument;
}

SvgGraph::~SvgGraph()
{
	if (m_dom != NULL)
	{
		delete m_dom;
		m_dom = NULL;
	}
	for (int i = 0;i<m_layerList.size();i++)
	{
		SvgLayer *player = m_layerList.at(i);
		delete player;
		player = NULL;
	}
}

QDomDocument* SvgGraph::getDom()
{
	return m_dom;
}

void SvgGraph::setId(QString id)
{
	m_id = id;
}

QString SvgGraph::getId()
{
	return m_id;
}

void SvgGraph::setFilePath(QString path)
{
	m_filePath = path;
}

QString SvgGraph::getFilePath()
{
	return m_filePath;
}

QList<SvgLayer*> & SvgGraph::getLayerList()
{
	return m_layerList;
}

QMap<QString,QString>& SvgGraph::getStyleMap()
{
	return m_styleMap;
}

QDomNode SvgGraph::getElementById(QString nodeid)
{
	QDomDocument* doc = getDom();
	QDomNodeList nodelist = doc->elementsByTagName(TAG_G);
	for (int i = 0;i<nodelist.size();i++)
	{
		QDomNode node = nodelist.at(i);
		QString layerid = node.toElement().attribute(ATTR_ID);
		if (layerid == BREAKER_LAYER
			|| layerid == DISCONN_LAYER
			|| layerid == GROUNDDISCONN_LAYER
			|| layerid == BUS_LAYER
			|| layerid == BUS_LAYER_JC
			|| layerid== LINK_LAYER
			|| layerid == ACLINE_LAYER
			|| layerid == CONNECTNODE_LAYER
			|| layerid == TRANS2_LAYER
			|| layerid == TRANS3_LAYER
			|| layerid == LOAD_LAYER
			)
		{
			QDomNodeList cnodelist = node.childNodes();
			for (int j = 0;j<cnodelist.size();j++)
			{
				QDomNode cnode = cnodelist.at(j);
				QString id = cnode.toElement().attribute(ATTR_ID);
				if ( id == nodeid)
				{
					return cnode;
				}
			}
		}
	}

	return QDomNode();
}

BaseDevice* SvgGraph::getDevById(QString svgid)
{
	for (int i = 0;i<m_layerList.size();i++)
	{
		SvgLayer* player = m_layerList.at(i);
		QString layerid = player->getId();
		if (layerid == BREAKER_LAYER
			|| layerid == DISCONN_LAYER
			|| layerid == GROUNDDISCONN_LAYER)
		{
			for (int j = 0;j<player->getDevList().size();j++)
			{
				BaseDevice* pdev = player->getDevList().at(j);
				if (pdev->getSvgId() == svgid)
				{
					return pdev;
				}
			}
		}
	}

	return NULL;
}

bool SvgGraph::setTransAttr(QString nodeid,QString symbid,QString attr,QString val)
{
	if (symbid.length()>0)
	{
		symbid = symbid.right(symbid.length()-1);
	}
	
	// 先删除symbol中的stroke
	QDomDocument *doc = this->getDom();

	QDomNodeList nodelist = doc->elementsByTagName(TAG_SYMBOL);
	for (int i = 0;i<nodelist.count();i++)
	{
		QDomNode cnode = nodelist.at(i);
		QString id = cnode.toElement().attribute(ATTR_ID);
		if (id == symbid )
		{
			QDomNodeList cnodelist = cnode.childNodes();
			for (int j = 0;j<cnodelist.count();j++)
			{
				QDomNode ccnode = cnodelist.at(j);
				if (ccnode.nodeName() == TAG_A)
				{
					QDomNode cccnode = ccnode.firstChild();
					cccnode.toElement().removeAttribute("stroke");
				}
				else if (ccnode.nodeName() == TAG_ELLIPSE)
				{
					ccnode.toElement().removeAttribute("stroke");
				}
			}
		}
	}

	return setAttribute(nodeid,attr,val);

}

bool SvgGraph::setAttribute(QString nodeid,QString attr,QString val)
{
	QDomNode cnode = getElementById(nodeid);
	if (cnode.isNull())
	{
		//qDebug("setAttribute failed! nodeid %s,attr:%s,val:%s",nodeid.toStdString().c_str(),attr.toStdString().c_str(),val.toStdString().c_str());
		return false;
	}
	// 图元节点的第一个子节点，比如设备图元的use节点，母线图元的path节点等
	QDomNode cn = cnode.firstChild();
	//if (cn.nodeName() == TAG_USE)
	{
		if (attr == ATTR_STYLE)
		{
			cn.toElement().removeAttribute("stroke");
		}
		
		cn.toElement().setAttribute(attr,val);
		return true;
	}
}

QString SvgGraph::getAttribute(QString nodeid,QString attr)
{
	QDomNode cnode = getElementById(nodeid);
	if (cnode.isNull())
	{
		//qDebug()<<"查找dom节点失败:"<<nodeid;
		return "";
	}

	QDomNode cn = cnode.firstChild();
	//if (cn.nodeName() == TAG_USE)
	{
		return cn.toElement().attribute(attr);
	}

	return "";
}