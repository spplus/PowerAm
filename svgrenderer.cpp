
#include <QTransform>
#include "svgrenderer.h"
#include "define.h"
#include "textitem.h"


SvgRenderer::SvgRenderer(QGraphicsScene* scene)
{
	m_scene = scene;
	m_graph = NULL;
	m_renderer = NULL;
}

SvgRenderer::~SvgRenderer()
{
	delete m_renderer;
	m_renderer = NULL;
}

QString SvgRenderer::initSvgRenderer()
{
	if (m_graph != NULL)
	{
		// 把symbol标签转成g标签
		QString sxml = m_graph->getDom()->toString();

		// 替换class属性为style属性
		sxml = sxml.replace("class","style");

		QMap<QString, QString>::const_iterator iter = m_graph->getStyleMap().constBegin();
		while (iter != m_graph->getStyleMap().constEnd()) 
		{
			QString key = iter.key();
			QString value = iter.value();

			sxml = sxml.replace(key,value);		
			iter ++;
		}
		
		return sxml;
	}
	else
	{
		return "";
	}
}

SvgItem* SvgRenderer::renderById(SvgGraph* graph,BaseDevice* pdev)
{
	SvgItem *item = NULL; 
	QRectF rect;
	if (graph != NULL && graph->getDom() != NULL)
	{
		QString sxml = initSvgRenderer();
		
		if( m_renderer->load(sxml.toUtf8()))
		{
			item = makeSvgItem(pdev->getSvgId());
			setItemPos(item,pdev);
		}
	}

	return item;
}

void SvgRenderer::drawGraph(SvgGraph* graph)
{
	m_graph = graph;

	// SVG文件格式转换
	m_renderer = new QSvgRenderer(initSvgRenderer().toUtf8());
	
	// 背景层 
	drawHeadLayer();

	// 文字层
	drawTextLayer();

	// 设备层
	for (int i = 0;i<m_graph->getLayerList().size();i++)
	{
		SvgLayer* layer = m_graph->getLayerList().at(i);
		QList<BaseDevice*>devList = layer->getDevList();

		for (int j = 0;j<devList.size();j++)
		{
			BaseDevice* pdev = devList.at(j);
			int id = pdev->getSvgId().toInt();
			SvgItem *item = addItem(pdev);
			if (item != NULL)
			{
				item->setLayerId(layer->getId());
			}
		}
	}

}

void SvgRenderer::drawHeadLayer()
{
	// 设置场景背景色
	SvgLayer *player = findLayer(HEAD_LAYER);
	if (player != NULL)
	{
		m_scene->setSceneRect(player->getX(),player->getY(),player->getWidth(),player->getHeight());
		QGraphicsSvgItem *item = addItem(player->getId());
		item->setZValue(-1);
		item->setFlag(QGraphicsItem::ItemIsSelectable,false);
	}
}

void SvgRenderer::drawTextLayer()
{
	
	SvgLayer* player = NULL;
	for (int i = 0;i<m_graph->getLayerList().size();i++)
	{
		player = m_graph->getLayerList().at(i);
		if (player->getId() == MEASURE_LAYER
			|| player->getId() == TEXT_LAYER  
			|| player->getId() == MEASURE2_LAYER)//
		{
				for (int i = 0;i<player->getTextList().size();i++)
				{
					TextSvg* ptext = player->getTextList().at(i);
					TextItem* item = new TextItem(*ptext);
					item->setPos(ptext->m_x,ptext->m_y);
					//item->setFlag(QGraphicsItem::ItemIsSelectable,false);
					m_scene->addItem(item);
				}
		}
	}

	
}


SvgItem* SvgRenderer::addItem(BaseDevice* pdev)
{
	if (pdev == NULL)
	{
		return NULL;
	}
	QString id = pdev->getSvgId();
	int iid = id.toInt();
	SvgItem* item = makeSvgItem(id);
	if (item != NULL)
	{
		// 设置生成后的item场景坐标
		setItemPos(item,pdev);

		// 设置item 类型
		item->setType(pdev->getDevType());

		m_scene->addItem(item);
	}

	return item;
}

void SvgRenderer::setItemPos(SvgItem* item,BaseDevice* pdev)
{
	qreal xp,yp;
	QRectF rect ;

	// 取边框矩形
	rect = m_renderer->boundsOnElement(pdev->getSvgId());

	// 取viewBox
	qreal vx=0,vy=0;
	QString symbolid = pdev->getSymbolId().right(pdev->getSymbolId().length()-1);
	m_renderer->getViewBoxOnElement(symbolid,vx,vy);

	// 判断是否需要坐标反转
	if (isReverseCoordination(pdev))
	{
		xp = rect.x()+vx;
		yp = rect.y()+vy;
	}
	else
	{
		xp = rect.x()-vx;
		yp = rect.y()-vy;
	}

	item->setPos(xp,yp);
}

SvgItem* SvgRenderer::addItem(QString id,eDeviceType tp /* = eDEFAULT */)
{
	if (id.length() <= 0)
	{
		return NULL;
	}
	SvgItem* item = makeSvgItem(id);
	if (item != NULL)
	{
		QRectF rect = m_renderer->boundsOnElement(id);
		qreal xp = rect.x();
		qreal yp = rect.y();
		item->setPos(xp,yp);

		// 设置item 类型
		item->setType(tp);

		m_scene->addItem(item);
	}
	
	return item;
}

SvgLayer* SvgRenderer::findLayer(QString lid)
{
	SvgLayer* player = NULL;
	for (int i = 0;i<m_graph->getLayerList().size();i++)
	{
		player = m_graph->getLayerList().at(i);
		if (player->getId() == lid)
		{
			return player;
		}
	}
	return player;
}

SvgItem* SvgRenderer::makeSvgItem(QString id)
{

	SvgItem* item = new SvgItem();
	item->setFlags( QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable);
	item->setSharedRenderer(m_renderer);
	item->setElementId(id.toLatin1());
	return item;
}

bool SvgRenderer::isReverseCoordination(BaseDevice* pdev)
{
	QString trans = pdev->getTransform();
	if (trans.length()<=0)
	{
		return false;
	}
	else
	{
		// 判断是否有rotate -180
		int idx = trans.indexOf("rotate");
		if (idx>=0)
		{
			QString angle = trans.mid(idx+6+1,4);
			if (angle.toInt() == -180)
			{
				return true;
			}
		}
		return false;
	}
}