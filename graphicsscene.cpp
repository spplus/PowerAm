

#include <QMessageBox>
#include <QGraphicsSceneMouseEvent>
#include <QPropertyAnimation>
#include <QStateMachine>
#include <QSignalTransition>
#include "graphicsscene.h"
#include "mainwindow.h"
#include "buff/msgbody.pb.h"
#include "linesetwidget.h"
#include "openwidget.h"
#include "savewidget.h"
#include "rulecheckwidget.h"

GraphicsScene::GraphicsScene(QObject* parant,QMenu* cntmenu)
	:QGraphicsScene(parant)
{
	m_svgRender = new SvgRenderer(this);
	m_cntMenu = cntmenu;
	m_curItem = NULL;
	m_curIndex = -1;
	m_saveId = 1;
	m_stationCim = "1";
	m_sysState = eANALOG;
	m_isOpFinished = true;
	m_boprflag = false;
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
	m_curItem = NULL;
	//clearItem();
}

SvgGraph* GraphicsScene::parserSvg(QString filename)
{
	return  m_svgParser.parserSvg(filename);
}

bool GraphicsScene::isOpened(QString fileName)
{
	// 判断是否已经打开过
	for (int i = 0;i<m_graphList.size();i++)
	{
		SvgGraph *pgraph = m_graphList.at(i);
		if (pgraph == NULL)
		{
			continue;
		}
		QString gname = pgraph->getFilePath();
		gname = gname.right(gname.length() - gname.indexOf("/")-1);

		if (gname == fileName)
		{
			m_svgRender->drawGraph(pgraph);
			return true;
		}
	}
	return false;
}

void GraphicsScene::drawSvgGraph(SvgGraph* pgrahp)
{
	m_opDevList.clear();
	if (pgrahp != NULL)
	{
		m_curItem = NULL;
		this->clear();
		m_svgRender->drawGraph(pgrahp);
		m_graphList.insert(++m_curIndex,pgrahp); 
	}
	else
	{
		QMessageBox::warning(views().at(0),MSG_TITLE,"打开图像文件失败");
	}
}

void GraphicsScene::openSvgFile(QString filename)
{
	m_opDevList.clear();
	SvgGraph *pgrahp = m_svgParser.parserSvg(filename);
	if (pgrahp != NULL)
	{
		m_curItem = NULL;
		this->clear();
		m_svgRender->drawGraph(pgrahp);
		m_graphList.insert(++m_curIndex,pgrahp); 
	}
}

void GraphicsScene::goNext()
{
	if (++m_curIndex < m_graphList.size() )
	{
		m_curItem = NULL;
		clear();
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
	if (--m_curIndex >= 0 )
	{
		m_curItem = NULL;
		clear();
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
		if (!m_isOpFinished)
		{
			QMessageBox::information(views().at(0),MSG_TITLE,"当前操作未结束，请稍后再试");
			return;
		}

		// 判断系统状态，只有在模拟态下才可以操作
		//当系统为拟票态时，在创建操作票的情况下右键才响应菜单
		if (m_sysState == eTICKET)
		{
			if (!ComUtil::instance()->getActionFlag())
			{
				return;
			}
		}

		SvgItem *item1= (SvgItem *)(this->itemAt(mouseEvent->scenePos()));
		if (item1 != NULL && item1->getCimId().length()>0)
		{
			PBNS::StateBean bean;
			if (findUnitBeanByCimId(item1->getCimId(),bean))
			{
				// 根据类型，创建菜单项
				MainWindow::instance()->addContextMenuAction(bean);

				// 保存当前选中的item；
				m_curItem = item1;
			}
			
		}
		else
		{
			m_cntMenu->clear();
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
	// 1. 发送操作命令

	// 2.后台判断是否允许校验，如果允许校验，则执行校验判断，并返回校验判断结果
	sendBreakOpReq(eOFF,MainWindow::instance()->getIsCheck());

	//switchChange(0);
}

void GraphicsScene::setClose()
{
	sendBreakOpReq(eON,MainWindow::instance()->getIsCheck());

	//switchChange(1);
}

void GraphicsScene::recvBreakerOpRes(const char* msg,int length)
{
	

	PBNS::OprationMsg_Response res;
	res.ParseFromArray(msg,length);
	
	// 把本次客户端操作的设备加入到操作设备列表
	putDev2OpList(m_curItem->getCimId(),res.optype());

	// 把服务器端自动设置的设备加入到操作设备列表
	putResList2OpList(res);

	// 把服务器端返回的进出线列表加入到本地列表中
	putResList2OpLine(res);

	if (ComUtil::instance()->getActionFlag())
	{
		m_curItem->getCimId();
		m_curItem->getType();
		QString strType = QString(tr("开关"));
		QString strState = QString(tr(""));
		if (m_curItem->getType() == eBREAKER || m_curItem->getType() == eSWITCH)
		{
			switch (m_curItem->getType())
			{
			case eBREAKER:
				strType = QString(tr("开关"));
				break;
			case eSWITCH:
				strType = QString(tr("刀闸"));
				break;
			}

			if (res.optype()==1)
			{
				strState = QString(tr("由分到合"));
			}
			else if (res.optype()==0)
			{
				strState = QString(tr("由合到分"));
			}

			QString strAct = QString(tr("%1 %2 %3 成功")).arg(m_curItem->getCimId()).arg(strType).arg(strState);
			TicketActionsMgr::instance()->setTicketActions(strAct);

		}
		
	}

	// 给带电的设备着色
	QList<PBNS::StateBean> blist = getStateBeanList(res);

	drawDev(blist);

	m_isOpFinished = true;
}

void GraphicsScene::putResList2OpLine(PBNS::OprationMsg_Response res)
{
	for (int i = 0;i<res.linelist_size();i++)
	{
		putLine2List(res.linelist(i));
	}
}

void GraphicsScene::putResList2OpList(PBNS::OprationMsg_Response res)
{
	for (int i = 0;i<res.oplist_size();i++)
	{
		PBNS::StateBean bean = res.oplist(i);
		putDev2OpList(bean.cimid().c_str(),bean.state());
	}
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
	newItem->setLayerId(m_curItem->getLayerId());

	// 从场景中删除
	removeItem(m_curItem);

	// 删除老Item
	//int ret = m_itemList.removeOne(m_curItem);
	QString c0 = m_curItem->getCimId();

	for (int i = 0;i<m_itemList.size();i++)
	{
		SvgItem* it = m_itemList.at(i);
		QString c1=it->getCimId();
		
		if (c1 == c0)
		{
			 m_itemList.removeAt(i);
			 break;
		}
	}
	
	addItem(newItem);

	// 插入新item
	m_itemList.push_back(newItem);

	m_curItem = newItem;

	// 保存当前变位的设备
	putDev2OpList(m_curItem->getCimId(),state);

}

QString GraphicsScene::getNewSymbolId(QString oldid,int state)
{
	// 截取最后一位
	return QString("%1%2").arg(oldid.mid(0,oldid.length()-1)).arg(state);
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
	PBNS::OprationMsg_Response res;
	res.ParseFromArray(msg,length);

	// 保存当前站点设备列表
	m_devList = res;

	// 把服务器端自动设置的设备加入到操作设备列表
	putResList2OpList(res);

	// 把服务器端返回的进出线列表加入到本地列表中
	putResList2OpLine(res);

	// 着色，变位
	drawDev(getStateBeanList(res));
}

bool GraphicsScene::setSvgStyle(SvgGraph* graph,QString svgId,QString style)
{
	QString color= tr("stroke:%1;fill:none").arg(style);
	if (graph != NULL)
	{
		return graph->setAttribute(svgId,ATTR_STYLE,color);
	}
	return false;
}

bool GraphicsScene::setTransColor(SvgGraph* graph,SvgItem* item,QString style)
{
	QString color= tr("stroke:%1;fill:none").arg(style);

	// 删除symbol中的stroke属性
	graph->setTransAttr(item->getSvgId(),item->getSymbolId(),ATTR_STYLE,color);

	return false;
}

void GraphicsScene::setBreakStateEx(SvgGraph* graph,QString svgid,QString cimid,eBreakerState state)
{
	//PBNS::StateBean bean;
	//int idx = findDevByCim(cimid,bean);

	//// 如果该元件在已操作列表，且不是本次操作的元件，则用操作列表中保持的状态进行显示
	//if (idx >= 0 && bean.cimid() != m_curItem->getCimId().toStdString())
	//{
	//	state = (eBreakerState)bean.state();
	//}


	// 1.通过svgid 找到对应的对应的设备对象中记录的symbolid
	QString oldid = graph->getAttribute(svgid,ATTR_XLINK);

	// 2.组建新状态下的模板ID
	QString symbolid = getNewSymbolId(oldid,state);

	// 3.修改dom中该节点的href属性
	if (graph != NULL)
	{
		graph->setAttribute(svgid,ATTR_XLINK,symbolid);
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

void GraphicsScene::colorDevEx(SvgGraph* graph,SvgItem* item,PBNS::StateBean &bean,QString color)
{
	eDeviceType dtype = item->getType();

	if (!item->getIsColor() )
	{
		if (dtype == eBREAKER 
			|| dtype == eSWITCH 
			|| dtype == eGROUNDSWITCH) 
		{
			PBNS::StateBean sbean;
			int idx = findDevByCim(m_opDevList,item->getCimId(),sbean);
			int state = 0;
			if (idx >=0)
			{
				state = sbean.state();
			}
			else
			{
				state = bean.state();
			}

			if ( state == 0)
			{
				color = POWEROFF_COLOR;
			}
		
			// 本设备着色
			setSvgStyle(graph,item->getSvgId(),color);
			item->setIsColor(true);
			item->setColor(color);
		}
		else if (dtype == eTRANSFORMER)
		{
			setTransColor(graph,item,color);
			item->setIsColor(true);
			item->setColor(color);
		}
		else
		{
			// 本设备着色
			setSvgStyle(graph,item->getSvgId(),color);
			item->setIsColor(true);
			item->setColor(color);
		}

	
	}
	else 
	{
		return;
	}
	
	//qDebug()<<ComUtil::instance()->now()<<"colorDev cim="<<item->getCimId();

	// 查找关联设备进行着色
	setConnectedDevColor(graph,item);
}


void GraphicsScene::colorDev(SvgGraph* graph,BaseDevice* pdev,PBNS::StateBean &bean,QString color)
{
	// 本设备着色
	setSvgStyle(graph,pdev->getSvgId(),color);

	// 查找item
	SvgItem* item = findSvgItemById(pdev->getSvgId());

	if (item != NULL && !item->getIsColor())
	{
		item->setIsColor(true);
		item->setColor(color);
	}
	else
	{
		return;
	}

	//qDebug()<<ComUtil::instance()->now()<<"colorDev cim="<<pdev->getMetaId();


	// 查找关联设备进行着色
	setConnectedDevColor(graph,item);
}

void GraphicsScene::setDevStateEx(QList<PBNS::StateBean>devlist,SvgGraph* graph)
{
	
	for (int i = 0;i<devlist.size();i++)
	{

		PBNS::StateBean bean = devlist.at(i);

		QString cim = "_EnergyConsumer_xdz10LD9109";
		
		int ck = cim.compare(bean.cimid().c_str());

		SvgItem *item = findSvgItemByCim(bean.cimid().c_str());
	
		if (item == NULL)
		{
			continue;
		}

		// 进出线在SVG中没有类型，通过unit表中的类型更新图元类型
		if (bean.unittype() == eLINE)
		{
			item->setType(eLINE);
		}

		eDeviceType unitType = item->getType();

		// 开关变位
		if (unitType == eSWITCH
			|| unitType == eBREAKER
			|| unitType == eGROUNDSWITCH)
		{
	
			PBNS::StateBean opbean ;
			int idx = findDevByCim(m_opDevList,item->getCimId(),opbean);

			// 如果该元件在已操作列表，且不是本次操作的元件，则用操作列表中保持的状态进行显示
			if (idx >= 0 && m_curItem != NULL 
				&& bean.cimid() != m_curItem->getCimId().toStdString())
			{
				bean.set_state(opbean.state());
			}

			setBreakStateEx(graph,item->getSvgId(),item->getCimId(),(eBreakerState)bean.state());
		}

		// 如果带电，则按配置的电压等级颜色进行着色
		if (bean.iselectric() == 1 && bean.isground() != 1)
		{
			colorDevEx(graph,item,bean,bean.volcolor().c_str());
		}
		// 带电接地
		else if (bean.iselectric() == 1 && bean.isground() == 1)
		{
			colorDevEx(graph,item,bean,POWERON_GROUND_COLOR);
		}
		// 不带电
		else if (bean.iselectric() == 0)
		{
			colorDevEx(graph,item,bean,POWEROFF_COLOR);
		}
		
	}
}


void GraphicsScene::setDevState(QList<PBNS::StateBean>devlist,SvgGraph* graph,BaseDevice* pdev)
{
	for (int i = 0;i<devlist.size();i++)
	{
		//qDebug()<<ComUtil::instance()->now()<<"setDevState i="<<i<<"devlist size="<<devlist.size();

		PBNS::StateBean bean = devlist.at(i);
		
		QString cim = "_BusbarSection_yaz110BUSⅠ";
		int  sid = cim.compare(pdev->getMetaId());
		
		if (bean.cimid().c_str() == pdev->getMetaId())
		{
			//qDebug()<<ComUtil::instance()->now()<<"find dev cim="<<pdev->getMetaId();

			// 进出线在SVG中没有类型，通过unit表中的类型更新图元类型
			if (bean.unittype() == eLINE)
			{
				pdev->setDevType(eLINE);
			}
			
			// 开关变位
			if (bean.unittype() == eSWITCH
				|| bean.unittype() == eBREAKER
				|| bean.unittype() == eGROUNDSWITCH)
			{
				setBreakState(graph,pdev,(eBreakerState)bean.state());
			}
			
			// 如果带电，则按配置的电压等级颜色进行着色
			if (bean.iselectric() == 1 && bean.isground() != 1)
			{
				colorDev(graph,pdev,bean,bean.volcolor().c_str());
			}
			// 带电接地
			else if (bean.iselectric() == 1 && bean.isground() == 1)
			{
				colorDev(graph,pdev,bean,POWERON_GROUND_COLOR);
			}
			// 不带电
			else if (bean.iselectric() == 0)
			{
				colorDev(graph,pdev,bean,POWEROFF_COLOR);
			}
		
		}
	}
}

void GraphicsScene::setConnectedDevColor(SvgGraph* pgraph,SvgItem* item)
{
	//QString dom =pgraph->getDom()->toString();

	if (item != NULL)
	{
		// 查找与该图元关联的图元
		QList<QGraphicsItem*> colist = item->collidingItems();
		for (int i = 0;i<colist.count();i++)
		{

			//qDebug()<<ComUtil::instance()->now()<<"setConnectedDevColor find ="<<colist.count()<<" i="<<i;

			SvgItem* coitem = (SvgItem*)colist.at(i);
			
			eDeviceType devtype = coitem->getType();
			if (devtype == eSWITCH 
				|| devtype == eBREAKER
				|| devtype == eGROUNDSWITCH
				|| devtype == eBUS
				|| devtype == eLINE
				|| devtype == eLOAD
				|| devtype == eDEFAULT
				|| devtype > eDEFAULT
				|| devtype == eTRANSFORMER
				|| coitem->getSvgId().length()==0)
			{
				continue;
			}
			if (!coitem->getIsColor())
			{

				setSvgStyle(pgraph,coitem->getSvgId(),item->getColor());
				coitem->setIsColor(true);
				coitem->setColor(item->getColor());
				
				//dom =pgraph->getDom()->toString();

				// 如果遇到变压器，则表示电压等级发送变化，不继续按原有电压等级进行关联着色
				if (devtype == eTRANSFORMER)
				{
					continue;
				}
				else
				{
					// 递归设置其关联的设备颜色
					setConnectedDevColor(pgraph,coitem);
				}
				
			}
		}

	}
}

SvgItem* GraphicsScene::findSvgItemByCim(QString cimid)
{
	// 获取当前场景中的全部item
	QList<SvgItem*>	sceneItems = m_itemList;//this->items();
	for(int i = 0;i<sceneItems.count();i++)
	{
		SvgItem* sitem = sceneItems.at(i);
		
		QString itemcim = sitem->getCimId();
		int rlst = itemcim.compare(cimid);
		//qDebug()<<"compare result:"<<rlst<<"sitem->getCimId()="<<sitem->getCimId()<<"cimid="<<cimid;
		if (rlst == 0)
		{
			return sitem;
		}
	}

	return NULL;
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
	for (int i = 0;i<m_opDevList.size();i++)
	{
		bean = m_opDevList.at(i);
		if (bean.cimid().c_str() == cimid)
		{
			return true;
		}
	}
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
		req.set_saveid(m_saveId);
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

void GraphicsScene::sendBreakOpReq(eBreakerState state,bool ischeck)
{
	m_isOpFinished = false;

	PBNS::OprationMsg_Request req;
	req.set_saveid(m_saveId);
	req.set_type(state);
	req.set_unitcim(m_curItem->getCimId().toStdString());
	req.set_unittype(m_curItem->getType());
	req.set_ischeck(ischeck);
	req.set_stationcim(m_stationCim.toStdString());

	// 设置操作设备列表
	getDevOpList(req);

	// 设置进出线列表
	getLineList(req);
	
	if (ComUtil::instance()->getActionFlag())
	{
		m_curItem->getCimId();
		m_curItem->getType();
		QString strType = QString(tr("开关"));
		QString strState = QString(tr(""));
		if (m_curItem->getType() == eBREAKER || m_curItem->getType() == eSWITCH)
		{
			switch (m_curItem->getType())
			{
			case eBREAKER:
				strType = QString(tr("开关"));
				break;
			case eSWITCH:
				strType = QString(tr("刀闸"));
				break;
			}

			if (state==eON)
			{
				strState = QString(tr("由分到合"));
			}
			else if (state==eOFF)
			{
				strState = QString(tr("由合到分"));
			}

			if (!m_boprflag)
			{
				QString strAct = QString(tr("%1 %2 %3")).arg(m_curItem->getCimId()).arg(strType).arg(strState);
				TicketActionsMgr::instance()->setTicketActions(strAct);
			}
		}
	}

	string data = req.SerializeAsString();
	NetClient::instance()->sendData(CMD_TOPO_BREAKER_CHANGE,data.c_str(),data.length());
}

void GraphicsScene::getDevOpList(PBNS::OprationMsg_Request& req)
{
	for (int i = 0;i<m_opDevList.size();i++)
	{
		PBNS::StateBean* opbean = req.add_opdevlist();
		opbean->CopyFrom(m_opDevList.at(i));
	}
}

void GraphicsScene::getLineList(PBNS::OprationMsg_Request& req)
{
	for (int i = 0;i<m_lineList.size();i++)
	{
		PBNS::StateBean* opbean = req.add_linelist();
		opbean->CopyFrom(m_lineList.at(i));
	}
}

void GraphicsScene::sendTagReq(eTagState type)
{
	PBNS::TagMsg_Request req;
	req.set_saveid(m_saveId);
	req.set_type(type);
	req.set_unitcim(m_curItem->getCimId() .toStdString());
	string data;
	req.SerializeToString(&data);
	NetClient::instance()->sendData(CMD_TAG_OP,data.c_str(),data.length());

	// 保存当前挂牌的设备
	PBNS::StateBean bean;
	if(findUnitBeanByCimId(m_curItem->getCimId(),bean))
	{
		bean.set_isboard(type);
		m_opDevList.append(bean);
	}

}

void GraphicsScene::readSaving()
{
	// 1.发送获取存档列表命令
	PBNS::SavingListMsg_Request req;
	req.set_id("0");
	string data;
	req.SerializeToString(&data);
	NetClient::instance()->sendData(CMD_READ_SAVING,data.c_str(),data.length());

}

void GraphicsScene::writeSaving()
{
	// 1.设置存档名称

	// 2.获取操作变位后的设备状态（开关状态，挂牌操作），发送到后台进行保存

	SaveWidget sdlg;
	if(sdlg.exec() == QDialog::Accepted)
	{
		QString sname = sdlg.getSaveName();
		PBNS::WriteSavingMsg_Request req;
		
		// 客户端操作设备的状态
		for (int i = 0;i<m_opDevList.size();i++)
		{
			PBNS::StateBean *bean = req.add_statelist();
			bean->CopyFrom(m_opDevList.at(i));
		}
		req.set_saveid(m_saveId);
		req.set_savename(sname.toStdString());
		string data = req.SerializeAsString();
		NetClient::instance()->sendData(CMD_WRITE_SAVING,data.c_str(),data.length());
	}
}


void GraphicsScene::showSavingList(const char* msg,int msglength)
{
	PBNS::SavingListMsg_Response res;
	res.ParseFromArray(msg,msglength);

	// 2.客户端显示存档列表窗口

	// 3.选择一个存档

	// 4.获取该存档下面设备状态

	// 5.着色

	OpenWidget odlg;
	odlg.setData(res);
	if (odlg.exec() == QDialog::Accepted)
	{
		m_saveId = odlg.getSaveId();

		// 获取该存档下的设备状态
		reqUnitState(m_stationCim);

		// 清空上一存档保持的操作设备集合
		m_opDevList.clear();
	}
}

void GraphicsScene::showRuleList(const char* msg,int length)
{
	PBNS::OprationMsg_Response res;
	res.ParseFromArray(msg,length);

	eBreakerState optype = (eBreakerState)res.optype();
	RuleCheckWidget dlg;
	dlg.setData(res);
	m_isOpFinished = true;
	
	//将触发的规则记录下来，写到操作票明细表中
	if (ComUtil::instance()->getActionFlag())
	{
		QList<QString> rulelist;
		rulelist = dlg.getRuleList();

		//只有开关或刀闸时写操作明细表
		if (m_curItem->getType() == eBREAKER || m_curItem->getType() == eSWITCH)
		{
			for (int i=0;i<rulelist.size();i++)
			{
				QString strRule = rulelist.at(i);
				QString strAct = QString(tr("触发规则:%1")).arg(strRule);
				///TicketActionsMgr::instance()->setTicketActions(strAct);
			}
		}
	}

	if (dlg.exec() == QDialog::Accepted)
	{
		//将触发的规则记录下来，写到操作票明细表中
		if (ComUtil::instance()->getActionFlag())
		{
			QList<QString> rulelist;
			rulelist = dlg.getRuleList();

			//只有开关或刀闸时写操作明细表
			if (m_curItem->getType() == eBREAKER || m_curItem->getType() == eSWITCH)
			{
				QString strAct = QString(tr("触发规则后继续执行"));
				///TicketActionsMgr::instance()->setTicketActions(strAct);
			}
		}
		m_boprflag = true;
		// 发送开关变位请求
		sendBreakOpReq(optype,false);
		m_boprflag = false;
	}
	else
	{
		//将触发的规则记录下来，写到操作票明细表中
		if (ComUtil::instance()->getActionFlag())
		{
			QList<QString> rulelist;
			rulelist = dlg.getRuleList();

			//只有开关或刀闸时写操作明细表
			if (m_curItem->getType() == eBREAKER || m_curItem->getType() == eSWITCH)
			{
				QString strAct = QString(tr("触发规则后取消操作"));
				///TicketActionsMgr::instance()->setTicketActions(strAct);
			}
		}
	}

}

void GraphicsScene::reqUnitState(QString stationCim)
{
	// 保存当前站点ID
	m_stationCim = stationCim;

	// 加载设备状态数据
	//PBNS::DevStateMsg_Request req;
	PBNS::OprationMsg_Request req;
	getLineList(req);

	req.set_saveid(m_saveId);
	req.set_stationcim(stationCim.toStdString());
	string reqstr;
	req.SerializeToString(&reqstr);
	NetClient::instance()->sendData(CMD_DEV_STATE,reqstr.c_str(),reqstr.length());
}

QList<PBNS::StateBean> GraphicsScene::getStateBeanList(PBNS::DevStateMsg_Response& res)
{
	QList<PBNS::StateBean> devlist;
	for (int i = 0;i<res.devstate_size();i++)
	{
		devlist.push_back(res.devstate(i));
	}
	return devlist;
}

QList<PBNS::StateBean> GraphicsScene::getStateBeanList(PBNS::OprationMsg_Response& res)
{
	QList<PBNS::StateBean> devlist;
	for (int i = 0;i<res.devstate_size();i++)
	{
		devlist.push_back(res.devstate(i));
	}

	return devlist;
}

void GraphicsScene::drawDev(QList<PBNS::StateBean>  stateList)
{
	if (stateList.size()<0)
	{
		return;
	}

	// 获取当前的图形
	if (m_graphList.size()<=m_curIndex || m_curIndex < 0)
	{
		return;
	}
	SvgGraph* pgraph = m_graphList.at(m_curIndex);

	setDevStateEx(stateList,pgraph);

	// 重新加载图形
	this->clear();
	//QString doms = pgraph->getDom()->toString();

	m_svgRender->drawGraph(pgraph);
}

void GraphicsScene::putItem(SvgItem* item)
{
	m_itemList.push_back(item);
}

void GraphicsScene::clearItem()
{
	m_itemList.clear();
	this->clear();
}

void GraphicsScene::putLine2List(PBNS::StateBean bean)
{
	PBNS::StateBean sbean;
	int idx = findDevByCim(m_lineList,bean.cimid().c_str(),sbean);
	if (idx > 0)
	{
		m_lineList.removeAt(idx);
	}
	else
	{
		m_lineList.push_back(bean);
	}

}

void GraphicsScene::putDev2OpList(QString cim,int state)
{
	// 查找是否已经存在
	PBNS::StateBean bean;
	int idx = findDevByCim(m_opDevList,cim,bean);
	if (idx>=0)
	{
		m_opDevList.removeAt(idx);
	}
	else
	{
		findUnitBeanByCimId(cim,bean);
	}
	// 更新状态
	bean.set_state(state);

	// 保持新状态
	m_opDevList.push_back(bean);
}

int GraphicsScene::findDevByCim(QList<PBNS::StateBean>dlist,QString cim,PBNS::StateBean & sbean)
{
	for (int i = 0;i<dlist.size();i++)
	{
		sbean = dlist.at(i);
		if (sbean.cimid() == cim.toStdString())
		{
			return i;
		}
	}
	return -1;
}

void GraphicsScene::setSysState(eSysState st)
{
		m_sysState = st;
}
