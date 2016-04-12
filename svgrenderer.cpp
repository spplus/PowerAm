
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
		// ��symbol��ǩת��g��ǩ
		QString sxml = m_graph->getDom()->toString();
		//sxml = sxml.replace("symbol","g");

		// �滻class����Ϊstyle����
		sxml = sxml.replace("class","style");

		// �滻��
		//sxml = sxml.replace("��","-");

		// �滻��
		//sxml = sxml.replace("��","R");

		
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

	// SVG�ļ���ʽת��
	m_renderer = new QSvgRenderer(initSvgRenderer().toUtf8());
	
	// ������ 
	drawHeadLayer();

	// ���ֲ�
	drawTextLayer();

	// �豸��
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
	// ���ó�������ɫ
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
		qreal xp,yp;
		QRectF rect ;

		// ȡ�߿����
		rect = m_renderer->boundsOnElement(id);

		// ȡviewBox
		qreal vx=0,vy=0;
		QString symbolid = pdev->getSymbolId().right(pdev->getSymbolId().length()-1);
		m_renderer->getViewBoxOnElement(symbolid,vx,vy);

		// �ж��Ƿ���Ҫ���귴ת
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

		// ����item ����
		item->setType(pdev->getDevType());

		m_scene->addItem(item);
	}

	return item;
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

		// ����item ����
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
		// �ж��Ƿ���rotate -180
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