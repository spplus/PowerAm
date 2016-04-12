
#include <QFile>
#include <QUuid>
#include <QDomCDATASection>
#include <QMessageBox>
#include "svgparser.h"
#include "define.h"
#include "transformer.h"

SvgParser::SvgParser()
{

}

SvgParser::~SvgParser()
{
	
}

bool SvgParser::openSvg(QString fileName,QDomDocument* doc)
{
	QFile *xmlFile;
	xmlFile = new QFile(fileName);

	if (!xmlFile->open(QIODevice::ReadOnly)){

		QString title = QString("打开%1文件").arg(fileName);
		QString text  = QString("打开%1文件,可能不存在该文件!").arg(fileName);
		QMessageBox::warning(NULL,title,text);
		return false;
	}

	if (!doc->setContent(xmlFile)) {

		QString title = QString("装载%1文件").arg(fileName);
		QString text  = QString("装载%1文件,可能该XML文件有错误!").arg(fileName);
		QMessageBox::warning(NULL,title,text);

		xmlFile->close();
		return false;
	}
	xmlFile->close();

	return true;
}

SvgGraph* SvgParser::parserSvg(QString filename)
{
	SvgGraph* pgraph = new SvgGraph;

	if (!openSvg(filename,pgraph->getDom()))
	{
		return NULL;
		delete pgraph;
	}
	// 保存文件名称
	pgraph->setFilePath(filename);

	QDomNodeList rootList = pgraph->getDom()->elementsByTagName(TAG_SVG);

	if (rootList.count()>0)
	{
		QDomNode childNode = rootList.at(0);

		QDomNodeList childList = childNode.childNodes();

		for (int i=0;i<childList.count();i++)
		{
			QDomNode node = childList.at(i);

			// 解析设备层
			if (node.nodeName() == TAG_G)
			{
				QString sid = getAttribute(node,ATTR_ID);
				if (sid == HEAD_LAYER)
				{
					parserHeader(pgraph,node);
				}
				else if (sid == BUS_LAYER)
				{
					parserBus(pgraph,node);
				}
				else if (sid == BREAKER_LAYER)
				{
					parserBreaker(pgraph,node);
				}
				else if (sid == DISCONN_LAYER)
				{
					parserDisconnector(pgraph,node);
				}
				else if (sid == GROUNDDISCONN_LAYER)
				{
					parserGroundDisconnector(pgraph,node);
				}
				else if (sid == LINK_LAYER 
					|| sid == ACLINE_LAYER
					|| sid == LOAD_LAYER
					|| sid == CONNECTNODE_LAYER)
				{
					parserLink(pgraph,node);
				}
				else if (sid == TRANS2_LAYER 
					|| sid == TRANS3_LAYER)
				{
					parserTransformer(pgraph,node);
				}
				else if (sid == TEXT_LAYER)
				{
					parserText(pgraph,node);
				}
				else if (sid == MEASURE_LAYER
					|| sid == MEASURE2_LAYER)
				{
					parserMeasure(pgraph,node);
				}
				else								// 公共use 设备图元
				{
					parserComDev(pgraph,node);
				}
			}
			else if (node.nodeName() == TAG_DEFS)			// 解析模板定义
			{
				QDomNodeList cnodes = node.childNodes();
				for(int i = 0;i<cnodes.count();i++)
				{
					QDomNode fnode = cnodes.at(i);
					QString nodename = fnode.nodeName();

					// 解析CSS
					if (nodename == TAG_STYLE)
					{
						parserStyle(fnode,pgraph->getStyleMap());
					}
				}
			}
			
		}
	}
	return pgraph;
}

void SvgParser::parserStyle(QDomNode& node,QMap<QString,QString>& sMap)
{
	if(node.isElement())
	{
		QString styleText = node.toElement().text();
		QStringList cssList = styleText.split(".");
		int size = cssList.size();
		QString cssText ;
		for (int i = 0;i<size;i++)
		{
			cssText = cssList.at(i).trimmed();
			if (cssText.length()>0)
			{
				// .kv0{stroke:rgb(170,255,0);fill:none}
				int idx1 = cssText.indexOf("{");
				int idx2 = cssText.indexOf("}");
				QString key = cssText.left(idx1).trimmed();
				QString value = cssText.mid(idx1+1,idx2-idx1-1);
				sMap.insert(key,value);

			}
		}
	}
}

void SvgParser::parserHeader(SvgGraph*grahp,QDomNode &node)
{
	SvgLayer *player = new SvgLayer;
	player->setId(getAttribute(node,ATTR_ID));
	
	if (node.hasChildNodes())
	{
		QDomNode cnode = node.firstChild();
		player->setFill(getAttribute(cnode,ATTR_FILL));
		player->setX(getAttribute(cnode,ATTR_X).toInt());
		player->setY(getAttribute(cnode,ATTR_Y).toInt());
		player->setHeight(getAttribute(cnode,ATTR_HEIGHT).toInt());
		player->setWidth(getAttribute(cnode,ATTR_WIDTH).toInt());
	}
	
	grahp->getLayerList().append(player);
}

void SvgParser::parserBus(SvgGraph*grahp,QDomNode &node)
{
	parserSvgLayer(grahp,node,eBUS);
}

void SvgParser::parserComDev(SvgGraph*grahp,QDomNode &node)
{
	parserSvgLayer(grahp,node,eDEFAULT);
}

void SvgParser::parserBreaker(SvgGraph*grahp,QDomNode &node)
{
	parserSvgLayer(grahp,node,eSWITCH);
}

void SvgParser::parserDisconnector(SvgGraph*grahp,QDomNode &node)
{
	parserSvgLayer(grahp,node,eSWITCH);
}

void SvgParser::parserGroundDisconnector(SvgGraph*grahp,QDomNode &node)
{
	parserSvgLayer(grahp,node,eSWITCH);
}

void SvgParser::parserLink(SvgGraph*grahp,QDomNode &node)
{
	parserSvgLayer(grahp,node,eLINK);
}

void SvgParser::parserTransformer(SvgGraph*grahp,QDomNode &node)
{
	parserSvgLayer(grahp,node,eTRANSFORMER);
}

void SvgParser::parserMeasure(SvgGraph* graph,QDomNode& node)
{
	SvgLayer *player = new SvgLayer;
	player->setId(getAttribute(node,ATTR_ID));
	QDomNodeList cnodelist = node.childNodes();
	for (int i=0;i<cnodelist.size();i++)
	{
		QDomNode cnode = cnodelist.at(i);
		QDomNode ccnode = cnode.firstChild();
		TextSvg* ptext = parserSvgText(ccnode);
		if (ptext != NULL)
		{
			player->getTextList().append(ptext);
		}
	}
	graph->getLayerList().append(player);
}

void SvgParser::parserText(SvgGraph*grahp,QDomNode &node)
{
	SvgLayer *player = new SvgLayer;
	player->setId(getAttribute(node,ATTR_ID));
	QDomNodeList cnodelist = node.childNodes();
	for (int i=0;i<cnodelist.size();i++)
	{
		QDomNode cnode = cnodelist.at(i);
		TextSvg* ptext = parserSvgText(cnode);
		if (ptext != NULL)
		{
			player->getTextList().append(ptext);
		}
	}

	grahp->getLayerList().append(player);
}

TextSvg * SvgParser::parserSvgText(QDomNode& cnode)
{
	if (cnode.nodeName() == TAG_TEXT)
	{
		TextSvg* ptext = new TextSvg;
		ptext->m_x = getAttribute(cnode,ATTR_X).toInt();
		ptext->m_y = getAttribute(cnode,ATTR_Y).toInt();
		ptext->m_fill = rgb2Qcolor(getAttribute(cnode,ATTR_FILL));
		ptext->m_fontFamily = getAttribute(cnode,ATTR_FONTFAMILY);
		ptext->m_fontHeight = getAttribute(cnode,ATTR_FONTH).toInt();
		ptext->m_fontWidth = getAttribute(cnode,ATTR_FONTW).toInt();
		ptext->m_fontSize = getAttribute(cnode,ATTR_FONTSIZE).toInt();
		ptext->m_stroke = rgb2Qcolor(getAttribute(cnode,ATTR_STROKE));
		ptext->m_transform = getAttribute(cnode,ATTR_TRANSFORM);
		ptext->m_text = cnode.toElement().text();
		return ptext;
	}
	return NULL;
}

QColor SvgParser::rgb2Qcolor(QString rgb)
{
	if (rgb.length()<=0)
	{
		return QColor();
	}
	int idx1 = rgb.indexOf("(");
	int idx2 = rgb.indexOf(")");
	if (idx1 < 0 || idx2 < 0)
	{
		return QColor();
	}

	QString cval = rgb.mid(idx1+1,idx2-idx1-1);
	QStringList rlist = cval.split(",");
	if (rlist.size() == 3)
	{
		return QColor(rlist.at(0).toInt(),rlist.at(1).toInt(),rlist.at(2).toInt());
	}
	else
	{
		return QColor();
	}
}

QString SvgParser::getAttribute(const QDomNode &node,QString attrname)
{
	if (node.toElement().hasAttribute(attrname))
	{
		return node.toElement().attribute(attrname);
	}
	else
	{
		return "";
	}
}

void SvgParser::parserMetaData(const QDomNode& ccnode,BaseDevice* pdev)
{
	if (ccnode.nodeName() == TAG_METADATA)
	{
		if (ccnode.hasChildNodes())
		{
			// 设备模型
			QDomNode cccnode = ccnode.firstChild();
			if (cccnode.nodeName() == TAG_PSR)
			{
				pdev->setMetaId( getAttribute(cccnode,ATTR_OBJECT_ID));
				pdev->setMetaName( getAttribute(cccnode,ATTR_OBJECT_NAME));
			}

			// 量测模型
			cccnode = cccnode.nextSibling();
			if (cccnode.nodeName() == TAG_MEAS)
			{
				pdev->setMeasId( getAttribute(cccnode,ATTR_OBJECT_ID));
			}

			// link 层模型数据，在此解析
			// ...

		}
	}
}

void SvgParser::parserUse(const QDomNode& ccnode,BaseDevice* pdev)
{
	if (ccnode.nodeName() == TAG_USE)
	{
		pdev->setSymbolId( getAttribute(ccnode,ATTR_XLINK));
		pdev->setX(getAttribute(ccnode,ATTR_X).toDouble());
		pdev->setY(getAttribute(ccnode,ATTR_Y).toDouble());
		pdev->setTransform(getAttribute(ccnode,ATTR_TRANSFORM));
		pdev->m_w = getAttribute(ccnode,ATTR_WIDTH).toDouble();
		pdev->m_h = getAttribute(ccnode,ATTR_HEIGHT).toDouble();
	}
}

void SvgParser::parserSvgLayer(SvgGraph* graph,const QDomNode & node,eDeviceType type)
{
	SvgLayer* player = new SvgLayer;
	
	player->setId(getAttribute(node,ATTR_ID));

	QDomNodeList cnodes = node.childNodes();
	//QList<BaseDevice*> dlist = player->getDevList();
	BaseDevice* pdev = NULL;
	for (int i = 0;i<cnodes.count();i++)
	{
		QDomNode cnode = cnodes.at(i);
		QString id = getAttribute(cnode,ATTR_ID);
		QString nodename = cnode.nodeName();

		switch (type)
		{
		case eBUS:
		case eLINK:
		case eACLINE:
		case eLOAD:
		case eTEXT:
			pdev = parserOriginal(cnode);
			
			
			break;
		case eSWITCH:
		case eARRESTER:
		case eDEFAULT:
			pdev = parserTemplate(cnode);
			
			break;
		case eTRANSFORMER:
			pdev = parserTransformer(cnode);
			break;
		}

		if (pdev != NULL)
		{
			pdev->setDevType(type);
			player->getDevList().append(pdev);
		}
	}
	graph->getLayerList().append(player);
}

BaseDevice* SvgParser::parserOriginal(const QDomNode& cnode)
{
	BaseDevice *pdev = new BaseDevice;
	
	QString id = getAttribute(cnode,ATTR_ID);
	if (id == "0" || id.length() == 0)
	{
		QString uuid = QUuid::createUuid().toString();
		cnode.toElement().setAttribute(ATTR_ID,uuid);
	}

	pdev->setSvgId(getAttribute(cnode,ATTR_ID));


	if (cnode.hasChildNodes())
	{
		QDomNode ccnode = cnode.firstChild();

		// 解析设备图形信息
		parserUse(ccnode,pdev);
		ccnode = ccnode.nextSibling();
		
		// 解析设备模型
		parserMetaData(ccnode,pdev);
	}
	return pdev;
}

BaseDevice* SvgParser::parserTemplate(const QDomNode& cnode)
{

	QString id = getAttribute(cnode,ATTR_ID);
	if (id == "0" )
	{
		QString uuid = QUuid::createUuid().toString();
		cnode.toElement().setAttribute(ATTR_ID,uuid);
	}

	if (cnode.hasChildNodes())
	{
		QDomNode ccnode = cnode.firstChild();
		if (ccnode.nodeName() != TAG_USE)
		{
			return NULL;
		}

		BaseDevice *pdev = new BaseDevice;
		pdev->setSvgId(getAttribute(cnode,ATTR_ID));

		// 解析设备图形信息
		parserUse(ccnode,pdev);
		ccnode = ccnode.nextSibling();

		// 解析设备模型
		parserMetaData(ccnode,pdev);

		return pdev;
	}
	return NULL;
}

BaseDevice* SvgParser::parserTransformer(const QDomNode& node)
{
	Transformer* ptrans = new Transformer;
	QString id = getAttribute(node,ATTR_ID);
	if (id == "0" || id.length() == 0)
	{
		QString uuid = QUuid::createUuid().toString();
		node.toElement().setAttribute(ATTR_ID,uuid);
	}

	ptrans->setSvgId(getAttribute(node,ATTR_ID));

	// 解析绕组
	QDomNodeList cnodelist = node.childNodes();
	for (int i = 0;i<cnodelist.size();i++)
	{
		QDomNode cnode = cnodelist.at(i);
		
		// kelong 变压器没有绕组，是整体
		parserUse(cnode,ptrans);

		ptrans->getWindList().push_back(parserTemplate(cnode));
	}

	return ptrans;
}