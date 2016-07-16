

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
		QMessageBox::information(views().at(0),MSG_TITLE,"��ǰ�Ѿ��������һ��λ��");
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
		QMessageBox::information(views().at(0),MSG_TITLE,"��ǰ�Ѿ����ڵ�һ��λ��");
	}
}

void GraphicsScene::mousePressEvent( QGraphicsSceneMouseEvent * mouseEvent )
{
	if (mouseEvent->button() == Qt::RightButton)
	{   
		SvgItem *item1= (SvgItem *)(this->itemAt(mouseEvent->scenePos()));
		if (item1 != NULL)
		{
			// �������ͣ������˵���
			MainWindow::instance()->addContextMenuAction(item1->getType());
			

			// ���浱ǰѡ�е�item��
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
	// 1. ���Ͳ�������

	// 2.��̨�ж��Ƿ�����У�飬�������У�飬��ִ��У���жϣ�������У���жϽ��
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
	
	// ִ��ͼ���е�ͼԪ���ر�λ
	//switchChange(res.optype());
	putDev2OpList(m_curItem->getCimId(),res.optype());

	// ��������豸��ɫ
	drawDev(getStateBeanList(res));
}

void GraphicsScene::switchChange(int state)
{
	// 1.ͨ����ǰѡ�е�item���ҵ�svgid
	if (m_curItem == NULL)
	{
		return;
	}

	// ��ȡ��ǰ��ͼ��
	if (m_graphList.size()<=m_curIndex)
	{
		return;
	}
	SvgGraph* pgraph = m_graphList.at(m_curIndex);

	QString svgid = m_curItem->elementId();

	// 2.ͨ��svgid �ҵ���Ӧ�Ķ�Ӧ���豸�����м�¼��symbolid
	QString oldid = pgraph->getAttribute(svgid,ATTR_XLINK);

	QString symbolid = getNewSymbolId(oldid,state);

	// 3.�޸�dom�иýڵ��href����
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

	// �ӳ�����ɾ��
	removeItem(m_curItem);

	// ɾ����Item
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

	// ������item
	m_itemList.push_back(newItem);

	m_curItem = newItem;

	// ���浱ǰ��λ���豸
	putDev2OpList(m_curItem->getCimId(),state);

}

QString GraphicsScene::getNewSymbolId(QString oldid,int state)
{
	// ��ȡ���һλ
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
	PBNS::DevStateMsg_Response res;
	res.ParseFromArray(msg,length);

	// ���浱ǰվ���豸�б�
	m_devList = res;

	qDebug()<<ComUtil::instance()->now()<<"��¼����"<<m_devList.devstate_size();

	// ��ɫ����λ
	drawDev(getStateBeanList(res));
// 
// 	// ����ͼ���е��豸״̬
// 	int size = res.devstate_size();
// 	if (size <= 0)
// 	{
// 		return;
// 	}
// 	
// 	// ��ȡ��ǰ��ͼ��
// 	if (m_graphList.size()<=m_curIndex)
// 	{
// 		return;
// 	}
// 	SvgGraph* pgraph = m_graphList.at(m_curIndex);
// 
// 	// ����cimid��ͼ�����ҵ���Ӧ���豸����
// 	for (int i = 0;i<pgraph->getLayerList().count();i++)
// 	{
// 		SvgLayer *player = pgraph->getLayerList().at(i);
// 		if (player->getId() == BREAKER_LAYER
// 			|| player->getId() == DISCONN_LAYER
// 			|| player->getId() == GROUNDDISCONN_LAYER
// 			|| player->getId() == BUS_LAYER
// 			|| player->getId() == BUS_LAYER_JC
// 			|| player->getId() == LINK_LAYER
// 			|| player->getId() == ACLINE_LAYER
// 			)
// 		{
// 			QList<BaseDevice*> devList = player->getDevList();
// 			for (int j = 0;j<devList.count();j++)
// 			{
// 				// ����svgid �޸�dom�е�ͼԪģ��,�ı�״̬��
// 				BaseDevice* pdev = devList.at(j);
// 				setDevState(res,pgraph,pdev);
// 			}
// 		}
// 	}
// 	// ���¼���ͼ��
// 	this->clear();
// 	
// 	m_svgRender->drawGraph(pgraph);
}

bool GraphicsScene::setSvgStyle(SvgGraph* graph,QString svgId,QString style)
{
	QString color= tr("stroke:%1").arg(style);
	if (graph != NULL)
	{
		return graph->setAttribute(svgId,ATTR_STYLE,color);
	}
	return false;
}

void GraphicsScene::setBreakStateEx(SvgGraph* graph,QString svgid,QString cimid,eBreakerState state)
{
	//PBNS::StateBean bean;
	//int idx = findDevByCim(cimid,bean);

	//// �����Ԫ�����Ѳ����б��Ҳ��Ǳ��β�����Ԫ�������ò����б��б��ֵ�״̬������ʾ
	//if (idx >= 0 && bean.cimid() != m_curItem->getCimId().toStdString())
	//{
	//	state = (eBreakerState)bean.state();
	//}


	// 1.ͨ��svgid �ҵ���Ӧ�Ķ�Ӧ���豸�����м�¼��symbolid
	QString oldid = graph->getAttribute(svgid,ATTR_XLINK);

	// 2.�齨��״̬�µ�ģ��ID
	QString symbolid = getNewSymbolId(oldid,state);

	// 3.�޸�dom�иýڵ��href����
	if (graph != NULL)
	{
		graph->setAttribute(svgid,ATTR_XLINK,symbolid);
	}

}


void GraphicsScene::setBreakState(SvgGraph* graph,BaseDevice* pdev,eBreakerState state)
{
	// 1.ͨ��svgid �ҵ���Ӧ�Ķ�Ӧ���豸�����м�¼��symbolid
	QString oldid = graph->getAttribute(pdev->getSvgId(),ATTR_XLINK);

	// 2.�齨��״̬�µ�ģ��ID
	QString symbolid = getNewSymbolId(oldid,state);

	// 3.�޸�dom�иýڵ��href����
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
			int idx = findDevByCim(item->getCimId(),sbean);
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
		
			// ���豸��ɫ
			setSvgStyle(graph,item->getSvgId(),color);
			item->setIsColor(true);
			item->setColor(color);
		}
		else
		{
			// ���豸��ɫ
			setSvgStyle(graph,item->getSvgId(),color);
			item->setIsColor(true);
			item->setColor(color);
		}

	
	}
	else 
	{
		return;
	}
	
	qDebug()<<ComUtil::instance()->now()<<"colorDev cim="<<item->getCimId();
	
	// ���ҹ����豸������ɫ
	setConnectedDevColor(graph,item);
}


void GraphicsScene::colorDev(SvgGraph* graph,BaseDevice* pdev,PBNS::StateBean &bean,QString color)
{
	// ���豸��ɫ
	setSvgStyle(graph,pdev->getSvgId(),color);

	// ����item
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

	qDebug()<<ComUtil::instance()->now()<<"colorDev cim="<<pdev->getMetaId();


	// ���ҹ����豸������ɫ
	setConnectedDevColor(graph,item);
}

void GraphicsScene::setDevStateEx(QList<PBNS::StateBean>devlist,SvgGraph* graph,BaseDevice* pdev)
{
	for (int i = 0;i<devlist.size();i++)
	{
		qDebug()<<ComUtil::instance()->now()<<"setDevState i="<<i<<"devlist size="<<devlist.size();
		
		PBNS::StateBean bean = devlist.at(i);

		QString cim = "_Breaker_hmkCB901G";
		
		int ck = cim.compare(bean.cimid().c_str());

		SvgItem *item = findSvgItemByCim(bean.cimid().c_str());
	
		if (item == NULL)
		{
			continue;
		}

		int  sid = cim.compare(item->getCimId());

		//if (bean.cimid().c_str() == pdev->getMetaId())
		{
			qDebug()<<ComUtil::instance()->now()<<"find dev cim="<<bean.cimid().c_str();

			// ��������SVG��û�����ͣ�ͨ��unit���е����͸���ͼԪ����
			if (bean.unittype() == eLINE)
			{
				//pdev->setDevType(eLINE);
				item->setType(eLINE);
			}

			// ���ر�λ
			if (bean.unittype() == eSWITCH
				|| bean.unittype() == eBREAKER
				|| bean.unittype() == eGROUNDSWITCH)
			{
	
				PBNS::StateBean opbean ;
				int idx = findDevByCim(item->getCimId(),opbean);

				// �����Ԫ�����Ѳ����б��Ҳ��Ǳ��β�����Ԫ�������ò����б��б��ֵ�״̬������ʾ
				if (idx >= 0 && bean.cimid() != m_curItem->getCimId().toStdString())
				{
					bean.set_state(opbean.state());
					bean.set_iselectric(opbean.state());
				}

				setBreakStateEx(graph,item->getSvgId(),item->getCimId(),(eBreakerState)bean.state());
			}

			// ������磬�����õĵ�ѹ�ȼ���ɫ������ɫ
			if (bean.iselectric() == 1 && bean.isground() != 1)
			{
				colorDevEx(graph,item,bean,bean.volcolor().c_str());
			}
			// ����ӵ�
			else if (bean.iselectric() == 1 && bean.isground() == 1)
			{
				colorDevEx(graph,item,bean,POWERON_GROUND_COLOR);
			}
			// ������
			else if (bean.iselectric() == 0)
			{
				colorDevEx(graph,item,bean,POWEROFF_COLOR);
			}

		}
	}
}


void GraphicsScene::setDevState(QList<PBNS::StateBean>devlist,SvgGraph* graph,BaseDevice* pdev)
{
	for (int i = 0;i<devlist.size();i++)
	{
		qDebug()<<ComUtil::instance()->now()<<"setDevState i="<<i<<"devlist size="<<devlist.size();

		PBNS::StateBean bean = devlist.at(i);
		
		QString cim = "_BusbarSection_yaz110BUS��";
		int  sid = cim.compare(pdev->getMetaId());
		
		//QString doms = graph->getDom()->toString();

		if (bean.cimid().c_str() == pdev->getMetaId())
		{
			qDebug()<<ComUtil::instance()->now()<<"find dev cim="<<pdev->getMetaId();

			// ��������SVG��û�����ͣ�ͨ��unit���е����͸���ͼԪ����
			if (bean.unittype() == eLINE)
			{
				pdev->setDevType(eLINE);
			}
			
			// ���ر�λ
			if (bean.unittype() == eSWITCH
				|| bean.unittype() == eBREAKER
				|| bean.unittype() == eGROUNDSWITCH)
			{
				setBreakState(graph,pdev,(eBreakerState)bean.state());
			}
			
			// ������磬�����õĵ�ѹ�ȼ���ɫ������ɫ
			if (bean.iselectric() == 1 && bean.isground() != 1)
			{
				colorDev(graph,pdev,bean,bean.volcolor().c_str());
			}
			// ����ӵ�
			else if (bean.iselectric() == 1 && bean.isground() == 1)
			{
				colorDev(graph,pdev,bean,POWERON_GROUND_COLOR);
			}
			// ������
			else if (bean.iselectric() == 0)
			{
				colorDev(graph,pdev,bean,POWEROFF_COLOR);
			}
		
		}
	}
}

void GraphicsScene::setConnectedDevColor(SvgGraph* pgraph,SvgItem* item)
{
	
	if (item != NULL)
	{
		// �������ͼԪ������ͼԪ
		QList<QGraphicsItem*> colist = item->collidingItems();
		for (int i = 0;i<colist.count();i++)
		{

			qDebug()<<ComUtil::instance()->now()<<"setConnectedDevColor find ="<<colist.count()<<" i="<<i;

			SvgItem* coitem = (SvgItem*)colist.at(i);
			
			eDeviceType devtype = coitem->getType();
			if (devtype == eSWITCH 
				|| devtype == eBREAKER
				|| devtype == eGROUNDSWITCH
				|| devtype == eBUS
				//|| devtype == eDEFAULT
				|| devtype > eDEFAULT
				|| coitem->getSvgId().length()==0)
			{
				continue;
			}
			if (!coitem->getIsColor())
			{

				setSvgStyle(pgraph,coitem->getSvgId(),item->getColor());
				coitem->setIsColor(true);
				coitem->setColor(item->getColor());
				
				// ���������ѹ�������ʾ��ѹ�ȼ����ͱ仯����������ԭ�е�ѹ�ȼ����й�����ɫ
				if (devtype == eTRANSFORMER)
				{
					continue;
				}
				else
				{
					// �ݹ�������������豸��ɫ
					setConnectedDevColor(pgraph,coitem);
				}
				
			}
		}

	}
}

SvgItem* GraphicsScene::findSvgItemByCim(QString cimid)
{
	

	// ��ȡ��ǰ�����е�ȫ��item
	QList<SvgItem*>	sceneItems = m_itemList;//this->items();
	for(int i = 0;i<sceneItems.count();i++)
	{
		SvgItem* sitem = sceneItems.at(i);
		
		QString itemcim = sitem->getCimId();
		int rlst = itemcim.compare(cimid);
		qDebug()<<"compare result:"<<rlst<<"sitem->getCimId()="<<sitem->getCimId()<<"cimid="<<cimid;
		if (rlst == 0)
		{
			return sitem;
		}
	}

	return NULL;
}

SvgItem* GraphicsScene::findSvgItemById(QString id)
{
	// ��ȡ��ǰ�����е�ȫ��item
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
		QMessageBox::warning(views().at(0),MSG_TITLE,tr("δ���ҵ���Ӧ���豸����:%1").arg(m_curItem->getCimId()));
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
	PBNS::OprationMsg_Request req;
	req.set_saveid(m_saveId);
	req.set_type(state);
	req.set_unitcim(m_curItem->getCimId().toStdString());
	req.set_unittype(m_curItem->getType());
	req.set_ischeck(ischeck);
	
	// ���ò����豸�б�
	
	for (int i = 0;i<m_opDevList.size();i++)
	{
		 PBNS::StateBean* opbean = req.add_opdevlist();
		 opbean->CopyFrom(m_opDevList.at(i));
	}

	string data = req.SerializeAsString();
	NetClient::instance()->sendData(CMD_TOPO_BREAKER_CHANGE,data.c_str(),data.length());
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

	// ���浱ǰ���Ƶ��豸
	PBNS::StateBean bean;
	if(findUnitBeanByCimId(m_curItem->getCimId(),bean))
	{
		bean.set_isboard(type);
		m_opDevList.append(bean);
	}

}

void GraphicsScene::readSaving()
{
	// 1.���ͻ�ȡ�浵�б�����
	PBNS::SavingListMsg_Request req;
	req.set_id("0");
	string data;
	req.SerializeToString(&data);
	NetClient::instance()->sendData(CMD_READ_SAVING,data.c_str(),data.length());

}

void GraphicsScene::writeSaving()
{
	// 1.���ô浵����

	// 2.��ȡ������λ����豸״̬������״̬�����Ʋ����������͵���̨���б���

	SaveWidget sdlg;
	if(sdlg.exec() == QDialog::Accepted)
	{
		QString sname = sdlg.getSaveName();
		PBNS::WriteSavingMsg_Request req;
		
		// �ͻ��˲����豸��״̬
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

	// 2.�ͻ�����ʾ�浵�б���

	// 3.ѡ��һ���浵

	// 4.��ȡ�ô浵�����豸״̬

	// 5.��ɫ

	OpenWidget odlg;
	odlg.setData(res);
	if (odlg.exec() == QDialog::Accepted)
	{
		m_saveId = odlg.getSaveId();

		// ��ȡ�ô浵�µ��豸״̬
		reqUnitState(m_stationCim);

		// �����һ�浵���ֵĲ����豸����
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
	if (dlg.exec() == QDialog::Accepted)
	{
		// ���Ϳ��ر�λ����
		sendBreakOpReq(optype,false);
	}

}

void GraphicsScene::reqUnitState(QString stationCim)
{
	// ���浱ǰվ��ID
	m_stationCim = stationCim;

	// �����豸״̬����
	PBNS::DevStateMsg_Request req;

	// ע�⣡����������վ��ID��saveId����Ҫ���ݴ򿪵�ͼ�δ�����������
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

void GraphicsScene::drawDev(QList<PBNS::StateBean> & stateList)
{
	if (stateList.size()<0)
	{
		return;
	}

	// ��ȡ��ǰ��ͼ��
	if (m_graphList.size()<=m_curIndex || m_curIndex < 0)
	{
		return;
	}
	SvgGraph* pgraph = m_graphList.at(m_curIndex);


	setDevStateEx(stateList,pgraph,NULL);

	/*

	// ����cimid��ͼ�����ҵ���Ӧ���豸����
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
			qDebug()<<ComUtil::instance()->now()<<"drawDev i="<<i;

			QList<BaseDevice*> devList = player->getDevList();
			for (int j = 0;j<devList.count();j++)
			{
				qDebug()<<ComUtil::instance()->now()<<"drawDev i="<<i<<" j="<<j<<" devList count="<<devList.count();
				// ����svgid �޸�dom�е�ͼԪģ��,�ı�״̬��
				BaseDevice* pdev = devList.at(j);
				setDevState(stateList,pgraph,pdev);
			}
		}
	}

	*/
	// ���¼���ͼ��
	this->clear();

	m_svgRender->drawGraph(pgraph);
}

void GraphicsScene::putItem(SvgItem* item)
{
	m_itemList.push_back(item);
}

void GraphicsScene::clearItem()
{
	m_itemList.clear();
}

void GraphicsScene::putDev2OpList(QString cim,int state)
{
	// �����Ƿ��Ѿ�����
	PBNS::StateBean bean;
	int idx = findDevByCim(cim,bean);
	if (idx>=0)
	{
		m_opDevList.removeAt(idx);
	}
	else
	{
		findUnitBeanByCimId(cim,bean);
	}
	// ����״̬
	bean.set_state(state);

	// ������״̬
	m_opDevList.push_back(bean);
}

int GraphicsScene::findDevByCim(QString cim,PBNS::StateBean & sbean)
{
	for (int i = 0;i<m_opDevList.size();i++)
	{
		sbean = m_opDevList.at(i);
		if (sbean.cimid() == cim.toStdString())
		{
			return i;
		}
	}
	return -1;
}