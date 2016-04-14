#include "mainwindow.h"
#include "svgparser.h"


#include <QFileDialog>
#include <QGraphicsTextItem>
#include <QGraphicsRectItem>
#include <QGraphicsSvgItem>
#include <QSvgRenderer>
#include <qfile.h>
#include <QMessageBox>
#include <QDomDocument>

#include "textitem.h"

MainWindow * MainWindow::m_self = 0;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
	m_self = this;
	m_title = "电力访误系统";
	initWidget();
	initToolBar();
	initMenu();
	setWindowTitle(m_title);
}

void MainWindow::initWidget()
{

	m_contextMenu = new QMenu(this);
	m_sence = new GraphicsScene(this,m_contextMenu);

	m_sence->setBackgroundBrush(QBrush(Qt::white));
	m_view = new GraphicsView(m_sence);

	//m_sence->setSceneRect(0,0,1500,1200);
	this->setGeometry(100, 100, 800, 500);
	setCentralWidget(m_view);
	this->showMaximized();
}

void MainWindow::initToolBar()
{
	initActions();
	m_toolBar =  this->addToolBar(tr("toolbar"));
	m_toolBar->addAction(m_viewModelAction);
	m_toolBar->addAction(m_openAction);
	m_toolBar->addAction(m_prevAction);
	m_toolBar->addAction(m_nextAction);
	m_toolBar->addAction(m_zoutAction);
	m_toolBar->addAction(m_zinAction);
	m_toolBar->addAction(m_originalAction);
	m_toolBar->addAction(m_refreshAction);
	m_toolBar->addAction(m_modelAction);

	m_toolBar->setIconSize(QSize(36,36));
}

void MainWindow::initMenu()
{
	m_sysMenu = this->menuBar()->addMenu(tr("系统&"));
	m_editMenu = this->menuBar()->addMenu(tr("设置&"));
	m_editMenu->addAction(m_userMgrAction);
	m_editMenu->addAction(m_stationTypeAction);
	m_editMenu->addAction(m_stationTypeAction);
	
	m_queryMenu = this->menuBar()->addMenu(tr("查询&"));
	m_queryMenu->addAction(m_circleQueryAction);
	m_queryMenu->addAction(m_cutQueryAction);
	m_queryMenu->addAction(m_gswitchQueryAction);
	m_queryMenu->addAction(m_msetQueryAction);
	m_queryMenu->addAction(m_signQueryAction);

}

void MainWindow::addContextMenuAction(eDeviceType type)
{
	m_contextMenu->clear();
	switch (type)
	{
	
	case eSWITCH:

		m_contextMenu->addAction(m_onAction);
		m_contextMenu->addAction(m_offAction);
		m_contextMenu->addAction(m_signOnAction);
		m_contextMenu->addAction(m_signOffAction);

		break;
	default:
		break;
	}
}

void MainWindow::initActions()
{
	
	m_openAction = new QAction(QIcon(":images/open.png"),tr("打开"),this);
	m_zoutAction	 = new QAction(QIcon(":images/zoomout.png"),tr("放大"),this);
	m_zinAction = new QAction(QIcon(":images/zoomin.png"),tr("缩小"),this);
	m_nextAction = new QAction(QIcon(":images/next.png"),tr("前进"),this);
	m_prevAction = new QAction(QIcon(":images/prev.png"),tr("后退"),this);
	m_userMgrAction = new QAction(QIcon(":images/usermgr.png"),tr("用户设置"),this);
	m_stationTypeAction = new QAction(QIcon(":images/stationmgr.png"),tr("电站类别设置"),this);
	m_stationRelaAction = new QAction(QIcon(":images/stationrela.png"),tr("电站设置"),this);
	m_modelAction = new QAction(QIcon(":images/model.png"),tr("运行模式"),this);
	m_refreshAction = new QAction(QIcon(":images/refresh.png"),tr("刷新数据"),this);
	m_circleQueryAction = new QAction(QIcon(":images/circlequery.png"),tr("环路查询"),this);
	m_signQueryAction = new QAction(QIcon(":images/signquery.png"),tr("挂牌查询"),this);
	m_gswitchQueryAction = new QAction(QIcon(":images/gswitchquery.png"),tr("地刀查询"),this);
	m_msetQueryAction = new QAction(QIcon(":images/msetquery.png"),tr("人工设置查询"),this);
	m_cutQueryAction = new QAction(QIcon(":images/cutquery.png"),tr("电路截面查询"),this);
	m_onAction = new QAction(QIcon(":images/on.png"),tr("置合"),this);
	m_offAction = new QAction(QIcon(":images/off.png"),tr("置分"),this);
	m_signOnAction = new QAction(QIcon(":images/signon.png"),tr("挂牌"),this);
	m_signOffAction = new QAction(QIcon(":images/signoff.png"),tr("摘牌"),this);
	m_viewModelAction = new QAction(QIcon(":images/pointer.png"),tr("指针"),this);
	m_originalAction = new QAction(QIcon(":images/zoom_original.png"),tr("原始尺寸"),this);

	// 1 指针 2 手掌
	m_viewModelAction->setData(QVariant(QGraphicsView::ScrollHandDrag));

	connect(m_openAction,SIGNAL(triggered()),this,SLOT(openFile()));
	connect(m_zinAction,SIGNAL(triggered()),m_view,SLOT(zoomIn()));
	connect(m_zoutAction,SIGNAL(triggered()),m_view,SLOT(zoomOut()));
	connect(m_prevAction,SIGNAL(triggered()),m_sence,SLOT(goPrev()));
	connect(m_nextAction,SIGNAL(triggered()),m_sence,SLOT(goNext()));
	connect(m_originalAction,SIGNAL(triggered()),m_view,SLOT(zoomHome()));
	connect(m_viewModelAction,SIGNAL(triggered()),this,SLOT(setViewModel()));
	connect(m_offAction,SIGNAL(triggered()),m_sence,SLOT(setOpen()));
	connect(m_onAction,SIGNAL(triggered()),m_sence,SLOT(setClose()));
	connect(m_refreshAction,SIGNAL(triggered()),m_sence,SLOT(startAnimation()));
}

MainWindow::~MainWindow()
{

}

void MainWindow::openFile()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("打开文件"),"/", tr("*.svg"));
	m_sence->openSvgFile(fileName);
	int idx = fileName.lastIndexOf("/");
	fileName = fileName.right(fileName.length()-idx-1);
	fileName = m_title+"-"+fileName;
	this->setWindowTitle(fileName);
}

void MainWindow::setViewModel()
{
	QGraphicsView::DragMode md;
	QIcon icon;
	if (m_viewModelAction->data().toInt() == QGraphicsView::NoDrag)
	{
		md = QGraphicsView::ScrollHandDrag;
		m_viewModelAction->setIcon(QIcon(":images/hand.png"));
	}
	else
	{
		md = QGraphicsView::NoDrag;
		m_viewModelAction->setIcon(QIcon(":images/pointer.png"));
	}
	m_viewModelAction->setData(md);
	m_view->setDragMode(md);
}


void MainWindow::recvdata(int msgtype,const char* msg,int msglength)
{

}

void MainWindow::connected()
{

}

void MainWindow::disconnected()
{

}
