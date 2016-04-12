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
			|| layerid == GROUNDDISCONN_LAYER)
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

bool SvgGraph::setAttribute(QString nodeid,QString attr,QString val)
{
	QDomNode cnode = getElementById(nodeid);
	if (cnode.isNull())
	{
		return false;
	}
	QDomNode cn = cnode.firstChild();
	if (cn.nodeName() == TAG_USE)
	{
		cn.toElement().setAttribute(attr,val);
		return true;
	}
	return false;
}

QString SvgGraph::getAttribute(QString nodeid,QString attr)
{
	QDomNode cnode = getElementById(nodeid);
	if (cnode.isNull())
	{
		return "";
	}

	QDomNode cn = cnode.firstChild();
	if (cn.nodeName() == TAG_USE)
	{
		return cn.toElement().attribute(attr);
	}

	return "";
}