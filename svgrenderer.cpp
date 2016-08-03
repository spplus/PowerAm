
#include <QTransform>
#include "svgrenderer.h"
#include "define.h"
#include "textitem.h"
#include "comutil.h"
#include "graphicsscene.h"

SvgRenderer::SvgRenderer(GraphicsScene* scene)
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
			item->setCimId(pdev->getMetaId());
			item->setSvgId(pdev->getSvgId());
			setItemPos(item,pdev);
		}
	}

	return item;
}

void SvgRenderer::drawGraph(SvgGraph* graph)
{
	// 清空之前保存的Item
	m_scene->clearItem();

	m_graph = graph;

	// SVG文件格式转换
	if (m_renderer != NULL)
	{
		delete m_renderer;
		m_renderer = NULL;
	}
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

			// 非设备层，置底
			switch (pdev->getDevType())
			{
			case eBREAKER:
				item->setZValue(3);
				break;
			case eSWITCH:
				item->setZValue(2);
				break;
			case eGROUNDSWITCH:
				item->setZValue(1);
				break;
			case eLINE:
				item->setZValue(4);
				break;
			
			default:
				item->setZValue(-1);
				break;
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
			|| player->getId() == MEASURE2_LAYER
			|| player->getId() == MEASURE3_LAYER)//
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


SvgItem* SvgRenderer::addItem(BaseDevice* pdev)
{
	if (pdev == NULL)
	{
		return NULL;
	}
	QString id = pdev->getSvgId();
	int iid = pdev->getDevType();
	SvgItem* item = makeSvgItem(id);
	if (item != NULL)
	{
		// 设置生成后的item场景坐标
		setItemPos(item,pdev);

		// 设置item 类型
		item->setType(pdev->getDevType());

		// 设置cimid
		item->setCimId(pdev->getMetaId());

		// 设置svgid
		item->setSvgId(id);

		m_scene->addItem(item);
		m_scene->putItem(item);
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
	int rt =isReverseCoordination(pdev); 
	
	// 旋转-180
	if (rt == 1)
	{
		xp = rect.x()+vx;
		yp = rect.y()+vy;
	}
	// 旋转-90
	else if (rt == 2)
	{
		xp = rect.x()-vx;
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
		m_scene->putItem(item);
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
	item->setFlag(QGraphicsItem::ItemIsSelectable);
	item->setSharedRenderer(m_renderer);
	item->setElementId(id.toLatin1());
	return item;
}

int SvgRenderer::isReverseCoordination(BaseDevice* pdev)
{
	QString trans = pdev->getTransform();
	if (trans.length()<=0)
	{
		return 0;
	}
	else
	{
		// 判断是否有rotate -180
		int idx = trans.indexOf("rotate");
		if (idx>=0)
		{
			int idx1 = trans.indexOf("(");
			int idx2 = trans.lastIndexOf(")");

			QString angle = trans.mid(idx1+1,idx2-idx1-1);
			QStringList plist = angle.split(" ");
			if (plist.count()>0)
			{
				angle = plist.at(0);
			}
			else
			{
				return 0;
			}
			float ag = angle.toFloat();
			if (ag == -180)
			{
				return 1;
			}
			else if (ag == -90)
			{
				return 2;
			}

		}
		return 0;
	}
}