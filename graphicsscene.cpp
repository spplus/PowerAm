
#include <QMessageBox>
#include <QGraphicsSceneMouseEvent>
#include <QPropertyAnimation>
#include <QStateMachine>
#include <QSignalTransition>
#include "graphicsscene.h"
#include "mainwindow.h"

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
			MainWindow::m_self->addContextMenuAction(item1->getType());
			

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
	QPropertyAnimation animation(m_curItem, "ps");
	animation.setDuration(2000);
	animation.setStartValue(m_curItem->pos());
	animation.setEndValue(QPointF(600,800));
	//animation.start();

	QStateMachine *machine = new QStateMachine;

	QState *state1 = new QState(machine);
	state1->assignProperty(m_curItem, "ps", m_curItem->pos());
	machine->setInitialState(state1);

	QState *state2 = new QState(machine);
	state2->assignProperty(m_curItem, "ps", QPointF(250, 250));

	QSignalTransition *transition1 = state1->addTransition(m_curItem,SIGNAL(test()), state2);
	transition1->addAnimation(new QPropertyAnimation(m_curItem, "ps"));

	QSignalTransition *transition2 = state2->addTransition(m_curItem,
		SIGNAL(test()), state1);
	transition2->addAnimation(new QPropertyAnimation(m_curItem, "ps"));

	machine->start();

}