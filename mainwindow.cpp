#include "mainwindow.h"
#include "svgparser.h"

#include <QPushButton>
#include <QFileDialog>
#include <QGraphicsTextItem>
#include <QGraphicsRectItem>
#include <QGraphicsSvgItem>
#include <QSvgRenderer>
#include <qfile.h>
#include <QMessageBox>
#include <QDomDocument>
#include <QCoreApplication>
#include <QTextCodec>

#include "navmodel.h"
#include "navdelegate.h"
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
	initNavView();

	m_spliter = new QSplitter(this);
	
	m_contextMenu = new QMenu(this);
	m_sence = new GraphicsScene(this,m_contextMenu);

	m_sence->setBackgroundBrush(QBrush(Qt::white));
	m_view = new GraphicsView(m_sence);

	this->setGeometry(100, 100, 800, 500);
	m_spliter->addWidget(m_navview);
	m_spliter->addWidget(m_view);

	QList<int> sizes;
	sizes.append(200);
	sizes.append(20);
	m_spliter->setSizes(sizes);
	m_spliter->setStretchFactor(1,1);

	setCentralWidget(m_spliter);
	this->showMaximized();
}

void MainWindow::initNavView()
{
	m_navview = new NavView(this);
	NavModel* model = new NavModel(this);
	model->ReadDataFromConfig(QCoreApplication::applicationDirPath() + "/config.xml");
	NavDelegate* delegate = new NavDelegate(this);
	m_navview->setModel(model);
	m_navview->setItemDelegate(delegate);
	connect(m_navview, SIGNAL(doubleClicked(const QModelIndex &)), model, SLOT(Collapse(const QModelIndex&)));
	connect(model,SIGNAL(openFile(QString)),this,SLOT(openFile(QString)));
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
	m_sysMenu->addAction(m_downSvg);

	m_editMenu = this->menuBar()->addMenu(tr("设置&"));
	m_editMenu->addAction(m_userMgrAction);
	m_editMenu->addAction(m_stationTypeAction);
	m_editMenu->addAction(m_stationRelaAction);
	
	m_queryMenu = this->menuBar()->addMenu(tr("查询&"));
	m_queryMenu->addAction(m_circleQueryAction);
	m_queryMenu->addAction(m_cutQueryAction);
	m_queryMenu->addAction(m_gswitchQueryAction);
	m_queryMenu->addAction(m_msetQueryAction);
	m_queryMenu->addAction(m_signQueryAction);
	m_queryMenu->addAction(m_roleQueryAction);
	m_queryMenu->addAction(m_topoQueryAction);
	m_queryMenu->addAction(m_scadaLogQueryAction);
	m_queryMenu->addAction(m_eventQueryAction);
	m_queryMenu->addAction(m_intervalQueryAction);

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
	m_downSvg = new QAction(QIcon(":images/download.png"),tr("下载Svg"),this);


	m_roleQueryAction = new QAction(QIcon(":images/download.png"),tr("校验规则"),this);
	m_topoQueryAction = new QAction(QIcon(":images/download.png"),tr("拓扑查询"),this);
	m_intervalQueryAction = new QAction(QIcon(":images/download.png"),tr("间隔查询"),this);
	m_scadaLogQueryAction = new QAction(QIcon(":images/download.png"),tr("scada日志查询"),this);
	m_eventQueryAction	= new QAction(QIcon(":images/download.png"),tr("事件查询"),this);


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
	connect(m_downSvg,SIGNAL(triggered()),this,SLOT(showDownSvg()));
}

MainWindow::~MainWindow()
{

}

void MainWindow::openFile()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("打开文件"),"/", tr("*.svg"));
	openFile(fileName);
}

void MainWindow::openFile(QString fileName)
{
	if (fileName.length()== 0)
	{
		QMessageBox::warning(this,"系统提示","文件名称为空");
		return;
	}

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

void MainWindow::showDownSvg()
{
	m_ftpUtil.exec();
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
