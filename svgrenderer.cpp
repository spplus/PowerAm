
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
		sxml = sxml.replace("symbol","g");

		// 替换class属性为style属性
		sxml = sxml.replace("class","style");

		// 替换横
		sxml = sxml.replace("横","-");

		// 替换右
		sxml = sxml.replace("右","R");

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

SvgItem* SvgRenderer::renderById(SvgGraph* graph,QString id)
{
	SvgItem *item = NULL; 
	QRectF rect;
	if (graph != NULL && graph->getDom() != NULL)
	{
		QString sxml = initSvgRenderer();
		
		if( m_renderer->load(sxml.toLatin1()))
		{
			item = makeSvgItem(id);

			QRectF rect = m_renderer->getNodeTransformedBounds(id);
			item->setPos(rect.x(),rect.y());
		}
	}

	return item;
}

void SvgRenderer::drawGraph(SvgGraph* graph)
{
	m_graph = graph;

	// SVG文件格式转换
	m_renderer = new QSvgRenderer(initSvgRenderer().toLatin1());
	
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
			
			SvgItem *item = addItem(pdev->getSvgId(),pdev->getDevType());
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
		item->setFlag(QGraphicsItem::ItemIsSelectable,false);
	}
}

void SvgRenderer::drawTextLayer()
{
	
	SvgLayer* player = NULL;
	for (int i = 0;i<m_graph->getLayerList().size();i++)
	{
		player = m_graph->getLayerList().at(i);
		if (player->getId() == TEXT_LAYER 
			|| player->getId() == MEASURE_LAYER)
		{
				for (int i = 0;i<player->getTextList().size();i++)
				{
					TextSvg* ptext = player->getTextList().at(i);
					TextItem* item = new TextItem(*ptext);
					item->setPos(ptext->m_x,ptext->m_y);
					item->setFlag(QGraphicsItem::ItemIsSelectable,false);
					m_scene->addItem(item);
				}
		}
	}

	
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
		QRectF rect = m_renderer->getNodeTransformedBounds(id);
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
	//QGraphicsSvgItem* item = new QGraphicsSvgItem();
	SvgItem* item = new SvgItem();

	//item->setCacheMode(QGraphicsItem::DeviceCoordinateCache);
	item->setFlags( QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable);
	item->setSharedRenderer(m_renderer);
	item->setElementId(id.toLatin1());
	return item;
}
