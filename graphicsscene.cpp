
#include <QMessageBox>
#include <QGraphicsSceneMouseEvent>
#include <QPropertyAnimation>
#include <QStateMachine>
#include <QSignalTransition>
#include "graphicsscene.h"
#include "mainwindow.h"
#include "buff/msgbody.pb.h"
#include "linesetwidget.h"

GraphicsScene::GraphicsScene(QObject* parant,QMenu* cntmenu)
	:QGraphicsScene(parant)
{
	m_svgRender = new SvgRenderer(this);
	m_cntMenu = cntmenu;
	m_curItem = NULL;
	m_curIndex = -1;
	m_saveId = 0;
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
		QMessageBox::information(views().at(0),MSG_TITLE,"当前已经处于最后一幅位置");
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
		QMessageBox::information(views().at(0),MSG_TITLE,"当前已经处于第一幅位置");
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
	if (m_curItem == NULL)
	{
		return;
	}
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

	// 保存当前站点设备列表
	m_devList = res;

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
			|| player->getId() == LINK_LAYER
			|| player->getId() == ACLINE_LAYER
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

void GraphicsScene::setSvgStyle(SvgGraph* graph,QString svgId,QString style)
{
	QString color= tr("stroke:%1").arg(style);
	if (graph != NULL)
	{
		graph->setAttribute(svgId,ATTR_STYLE,color);
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
			// 进出线在SVG中没有类型，通过unit表中的类型更新图元类型
			if (bean.unittype() == eLINE)
			{
				pdev->setDevType(eLINE);
			}

			// 开关变位
			setBreakState(graph,pdev,(eBreakerState)bean.state());

			// 如果带电，则按配置的电压等级颜色进行着色
			if (bean.iselectric() == 1)
			{
				// 本设备着色
				QString style = tr("%1").arg(bean.volvalue().c_str());
				setSvgStyle(graph,pdev->getSvgId(),style);

				// 查找关联设备进行着色
				QString test = graph->getDom()->toString();
				// 查找item
				SvgItem* item = findSvgItemById(pdev->getSvgId());
				if (item != NULL)
				{
					item->setIsColor(true);
				}
				setConnectedDevColor(graph,item);
			}
		
		}
	}
}

void GraphicsScene::setConnectedDevColor(SvgGraph* pgraph,SvgItem* item)
{
	
	if (item != NULL)
	{
		// 查找与该图元关联的图元
		QList<QGraphicsItem*> colist = item->collidingItems();
		for (int i = 0;i<colist.count();i++)
		{
			SvgItem* coitem = (SvgItem*)colist.at(i);
			if (coitem->getType() == eSWITCH  
				|| coitem->getType() == eDEFAULT
				|| coitem->getSvgId().length()==0)
			{
				continue;
			}
			if (!coitem->getIsColor())
			{
				setSvgStyle(pgraph,coitem->getSvgId(),POWERON_COLOR);
				coitem->setIsColor(true);

				// 递归设置其关联的设备颜色
				setConnectedDevColor(pgraph,coitem);
			}
		}

	}
}

SvgItem* GraphicsScene::findSvgItemById(QString id)
{
	// 获取当前场景中的全部item
	QList<QGraphicsItem*>	sceneItems = this->items();
	for(int i = 0;i<sceneItems.count();i++)
	{
		SvgItem* sitem = (SvgItem*)sceneItems.at(i);
		if (sitem->getSvgId() == id)
		{
			return sitem;
		}
	}

	return NULL;
}


bool GraphicsScene::findUnitBeanByCimId(QString cimid,PBNS::StateBean & bean)
{
	for(int i = 0;i<m_devList.devstate_size();i++)
	{
		bean = m_devList.devstate(i);
		if (bean.cimid().c_str() == cimid)
		{
			return true;
		}
	}
	return false;
}

void GraphicsScene::setPower()
{
	if (m_curItem == NULL)
	{
		return;
	}
	
	PBNS::StateBean bean;
	if (findUnitBeanByCimId(m_curItem->getCimId(),bean))
	{
		PBNS::PowerSetMsg_Request req;
		req.set_unitcim(bean.cimid());
		req.set_stationcim(bean.stationcim());
		string data;
		req.SerializeToString(&data);
		NetClient::instance()->sendData(CMD_POWER_SET,data.c_str(),data.length());
	}
	else
	{
		QMessageBox::warning(views().at(0),MSG_TITLE,tr("未查找到对应的设备对象:%1").arg(m_curItem->getCimId()));
	}
}

void GraphicsScene::setLine()
{
	if (m_curItem == NULL)
	{
		return;
	}
	LineSetWidget dlg;
	if(dlg.exec() == QDialog::Accepted)
	{
		QString oneCim = dlg.getOneCim();
		QString ohrCim = dlg.getOhrCim();
		PBNS::LineSetMsg_Request req;
		req.set_unitcim(m_curItem->getCimId().toStdString());
		req.set_stationonecim(oneCim.toStdString());
		req.set_stationothercim(ohrCim.toStdString());

		string data;
		req.SerializeToString(&data);
		NetClient::instance()->sendData(CMD_LINE_SET,data.c_str(),data.length());
	}
}

void GraphicsScene::tagOff()
{
	if (m_curItem == NULL)
	{
		return;
	}

	sendTagReq(eTagOff);
}

void GraphicsScene::tagOn()
{
	if (m_curItem == NULL)
	{
		return;
	}

	sendTagReq(eTagOn);
}

void GraphicsScene::sendTagReq(eTagState type)
{
	PBNS::TagMsg_Request req;
	req.set_saveid(m_saveId);
	req.set_type(type);
	req.set_unitcim(m_curItem->getCimId().toStdString());
	string data;
	req.SerializeToString(&data);
	NetClient::instance()->sendData(CMD_TAG_OP,data.c_str(),data.length());
}