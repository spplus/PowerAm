
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
	QPixmap p(800,600);
	p.load(":/images/bk.jpg");

	m_bkItem = new QGraphicsPixmapItem;
	m_bkItem->setPixmap(p);
	addItem(m_bkItem);
	m_bkItem->setPos(0,0);
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
		if (m_bkItem != NULL)
		{
			this->removeItem(m_bkItem);
			delete m_bkItem;
			m_bkItem = NULL;
		}
		
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
		QMessageBox::information(views().at(0),"ϵͳ��ʾ","��ǰ�Ѿ��������һ��λ��");
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
		QMessageBox::information(views().at(0),"ϵͳ��ʾ","��ǰ�Ѿ����ڵ�һ��λ��");
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
			MainWindow::m_self->addContextMenuAction(item1->getType());
			

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
	switchChange(0);
}

void GraphicsScene::setClose()
{
	switchChange(1);
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

	removeItem(m_curItem);
	addItem(newItem);

	m_curItem = newItem;
}

QString GraphicsScene::getNewSymbolId(QString oldid,int state)
{
	// ��ȡ���һλ
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

	// ����ͼ���е��豸״̬
	int size = res.devstate_size();
	PBNS::StateBean bean = res.devstate(0);
	
	// ����cimid��ͼ�����ҵ���Ӧ���豸����

	// ����svgid �޸�dom�е�ͼԪģ��,�ı�״̬��

	// ���¼���ͼ��


}