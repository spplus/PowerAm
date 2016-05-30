
#include <QMessageBox>
#include <QGraphicsSceneMouseEvent>
#include <QPropertyAnimation>
#include <QStateMachine>
#include <QSignalTransition>
#include "graphicsscene.h"
#include "mainwindow.h"
#include "buff/msgbody.pb.h"

GraphicsScene::GraphicsScene(QObject* parant,QMenu* cntmenu)
	:QGraphicsScene(parant)
{
	m_svgRender = new SvgRenderer(this);
	m_cntMenu = cntmenu;
	m_curItem = NULL;
	m_curIndex = -1;

}

GraphicsScene::~GraphicsScene()
{
	for (int i = 0;i<m_graphList.size();i++)
	{
		SvgGraph* pg = m_graphList.at(i);
		delete pg;
		pg = NULL;
	}
	m_graphList.clear();

	if (m_svgRender != NULL)
	{
		delete m_svgRender;
		m_svgRender = NULL;
	}
}

SvgGraph* GraphicsScene::parserSvg(QString filename)
{
	return  m_svgParser.parserSvg(filename);
}

void GraphicsScene::drawSvgGraph(SvgGraph* pgrahp)
{
	if (pgrahp != NULL)
	{

		this->clear();
		m_svgRender->drawGraph(pgrahp);
		m_graphList.insert(++m_curIndex,pgrahp); 
	}
}

void GraphicsScene::openSvgFile(QString filename)
{

	SvgGraph *pgrahp = m_svgParser.parserSvg(filename);
	if (pgrahp != NULL)
	{

		this->clear();
		m_svgRender->drawGraph(pgrahp);
		m_graphList.insert(++m_curIndex,pgrahp); 
	}
}

void GraphicsScene::goNext()
{
	clear();
	if (++m_curIndex < m_graphList.size() )
	{
		SvgGraph* pgraph = m_graphList.at(m_curIndex);
		m_svgRender->drawGraph(pgraph);
	}
	else
	{
		m_curIndex--;
		QMessageBox::information(views().at(0),"系统提示","当前已经处于最后一幅位置");
	}
}

void GraphicsScene::goPrev()
{
	clear();
	if (--m_curIndex >= 0 )
	{
		SvgGraph* pgraph = m_graphList.at(m_curIndex);
		m_svgRender->drawGraph(pgraph);
	}
	else
	{
		m_curIndex++;
		QMessageBox::information(views().at(0),"系统提示","当前已经处于第一幅位置");
	}
}

void GraphicsScene::mousePressEvent( QGraphicsSceneMouseEvent * mouseEvent )
{
	if (mouseEvent->button() == Qt::RightButton)
	{   
		SvgItem *item1= (SvgItem *)(this->itemAt(mouseEvent->scenePos()));
		if (item1 != NULL)
		{
			// 根据类型，创建菜单项
			MainWindow::instance()->addContextMenuAction(item1->getType());
			

			// 保存当前选中的item；
			m_curItem = item1;
		}
		
		return ;
	}
}

void GraphicsScene::contextMenuEvent ( QGraphicsSceneContextMenuEvent * contextMenuEvent )
{
	m_cntMenu->exec(contextMenuEvent->screenPos());
	QGraphicsScene::contextMenuEvent(contextMenuEvent);
}

void GraphicsScene::setOpen()
{
	switchChange(0);
}

void GraphicsScene::setClose()
{
	switchChange(1);
}

void GraphicsScene::switchChange(int state)
{
	// 1.通过当前选中的item，找到svgid
	if (m_curItem == NULL)
	{
		return;
	}

	// 获取当前的图形
	if (m_graphList.size()<=m_curIndex)
	{
		return;
	}
	SvgGraph* pgraph = m_graphList.at(m_curIndex);

	QString svgid = m_curItem->elementId();

	// 2.通过svgid 找到对应的对应的设备对象中记录的symbolid
	QString oldid = pgraph->getAttribute(svgid,ATTR_XLINK);

	QString symbolid = getNewSymbolId(oldid,state);

	// 3.修改dom中该节点的href属性
	if (pgraph != NULL)
	{
		pgraph->setAttribute(svgid,ATTR_XLINK,symbolid);
	}
	else
	{
		return;
	}

	BaseDevice* pdev = pgraph->getDevById(svgid);
	if (pdev == NULL)
	{
		return;
	}
	SvgItem* newItem = m_svgRender->renderById(pgraph,pdev);

	newItem->setType(m_curItem->getType());

	removeItem(m_curItem);
	addItem(newItem);

	m_curItem = newItem;
}

QString GraphicsScene::getNewSymbolId(QString oldid,int state)
{
	// 截取最后一位
	//int state = oldid.mid(oldid.length()-1,1).toInt();

	//state = state==0?1:0;
	
	QString newid = QString("%1%2").arg(oldid.mid(0,oldid.length()-1)).arg(state);
	return newid;
}

void GraphicsScene::startAnimation()
{
	m_animation.setTargetObject(m_curItem);
	m_animation.setPropertyName("pos");
	m_animation.setDuration(2000);
	m_animation.setStartValue(QPointF(0,0));
	m_animation.setEndValue(m_curItem->pos());
	m_animation.setEasingCurve(QEasingCurve::OutBounce);
	m_animation.start();
}

void GraphicsScene::showDevState(const char* msg,int length)
{
	PBNS::DevStateMsg_Response res;
	res.ParseFromArray(msg,length);

	// 设置图形中的设备状态
	int size = res.devstate_size();
	if (size <= 0)
	{
		return;
	}
	
	// 获取当前的图形
	if (m_graphList.size()<=m_curIndex)
	{
		return;
	}
	SvgGraph* pgraph = m_graphList.at(m_curIndex);

	// 根据cimid在图形中找到对应的设备对象
	for (int i = 0;i<pgraph->getLayerList().count();i++)
	{
		SvgLayer *player = pgraph->getLayerList().at(i);
		if (player->getId() == BREAKER_LAYER
			|| player->getId() == DISCONN_LAYER
			|| player->getId() == GROUNDDISCONN_LAYER
			|| player->getId() == BUS_LAYER
			|| player->getId() == BUS_LAYER_JC
			)
		{
			QList<BaseDevice*> devList = player->getDevList();
			for (int j = 0;j<devList.count();j++)
			{
				// 根据svgid 修改dom中的图元模板,改变状态。
				BaseDevice* pdev = devList.at(j);
				setDevState(res,pgraph,pdev);
			}
		}
	}
	// 重新加载图形
	this->clear();
	m_svgRender->drawGraph(pgraph);
}

void GraphicsScene::setSvgStyle(SvgGraph* graph,BaseDevice* pdev,QString style)
{
	if (graph != NULL)
	{
		QString old = graph->getAttribute(pdev->getSvgId(),ATTR_STYLE);

		graph->setAttribute(pdev->getSvgId(),ATTR_STYLE,style);
	}
}
void GraphicsScene::setBreakState(SvgGraph* graph,BaseDevice* pdev,eBreakerState state)
{
	// 1.通过svgid 找到对应的对应的设备对象中记录的symbolid
	QString oldid = graph->getAttribute(pdev->getSvgId(),ATTR_XLINK);

	// 2.组建新状态下的模板ID
	QString symbolid = getNewSymbolId(oldid,state);

	// 3.修改dom中该节点的href属性
	if (graph != NULL)
	{
		graph->setAttribute(pdev->getSvgId(),ATTR_XLINK,symbolid);
	}
}

void GraphicsScene::setDevState(PBNS::DevStateMsg_Response &res,SvgGraph* graph,BaseDevice* pdev)
{
	for (int i = 0;i<res.devstate_size();i++)
	{
		PBNS::StateBean bean = res.devstate(i);
		if (bean.cimid().c_str() == pdev->getMetaId())
		{
			// 开关变位
			setBreakState(graph,pdev,(eBreakerState)bean.state());

			// 着色
			QString style = tr("stroke:%1").arg(bean.volvalue().c_str());
			setSvgStyle(graph,pdev,style);
		}
	}
}