#include "mainwindow.h"
#include "svgparser.h"
#include <string>
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
#include "homewindow.h"
#include "navmodel.h"
#include "navdelegate.h"
#include "textitem.h"
#include "buff/msgbody.pb.h"
#include "include/commands.h"

using namespace std;
MainWindow * MainWindow::m_inst = NULL;

MainWindow* MainWindow::instance()
{
	if (m_inst == NULL)
	{
		m_inst = new MainWindow;
	}
	return m_inst;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
	
	m_title = "电力访误系统";
	initWidget();
	initToolBar();
	initMenu();
	initStatusBar();
	//初始化网络连接
	initNet();

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
	m_toolBar->addAction(m_homeAction);
	m_toolBar->addAction(m_viewModelAction);
	m_toolBar->addAction(m_openAction);
	m_toolBar->addAction(m_prevAction);
	m_toolBar->addAction(m_nextAction);
	m_toolBar->addAction(m_originalAction);
	m_toolBar->addAction(m_zoutAction);
	m_toolBar->addAction(m_zinAction);
	m_toolBar->addAction(m_refreshAction);
	m_toolBar->addAction(m_saveAction);
	m_toolBar->addAction(m_readAction);
	m_toolBar->addAction(m_controlAction);
	m_toolBar->addAction(m_chekAction);
	m_toolBar->addAction(m_netAction);
	m_toolBar->addAction(m_soundAction);
	m_toolBar->addAction(m_roleAction);
	m_toolBar->addAction(m_circleQueryAction);
	m_toolBar->addAction(m_signQueryAction);
	m_toolBar->addAction(m_gswitchQueryAction);
	m_toolBar->addAction(m_opQueryAction);
	m_toolBar->addAction(m_eventQueryAction);

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
	m_homeAction = new QAction(QIcon(":images/open.png"),tr("主页"),this);
	m_openAction = new QAction(QIcon(":images/open.png"),tr("打开"),this);
	m_zoutAction	 = new QAction(QIcon(":images/zoomout.png"),tr("放大"),this);
	m_zinAction = new QAction(QIcon(":images/zoomin.png"),tr("缩小"),this);
	m_nextAction = new QAction(QIcon(":images/next.png"),tr("前进"),this);
	m_prevAction = new QAction(QIcon(":images/prev.png"),tr("后退"),this);
	m_refreshAction = new QAction(QIcon(":images/refresh.png"),tr("刷新"),this);
	m_saveAction = new QAction(QIcon(":images/signquery.png"),tr("保存"),this);
	m_readAction = new QAction(QIcon(":images/signquery.png"),tr("读取"),this);
	m_controlAction = new QAction(QIcon(":images/signquery.png"),tr("控制"),this);
	m_chekAction = new QAction(QIcon(":images/signquery.png"),tr("校验"),this);
	m_netAction = new QAction(QIcon(":images/prev.png"),tr("网络"),this);
	m_soundAction = new QAction(QIcon(":images/prev.png"),tr("声音告警"),this);
	m_roleAction = new QAction(QIcon(":images/prev.png"),tr("规则设置"),this);
	m_circleQueryAction = new QAction(QIcon(":images/circlequery.png"),tr("环路查询"),this);
	m_signQueryAction = new QAction(QIcon(":images/signquery.png"),tr("挂牌查询"),this);
	m_gswitchQueryAction = new QAction(QIcon(":images/gswitchquery.png"),tr("接地查询"),this);
	m_opQueryAction = new QAction(QIcon(":images/gswitchquery.png"),tr("操作查询"),this);
	m_eventQueryAction	= new QAction(QIcon(":images/download.png"),tr("事件查询"),this);
	m_modelAction = new QAction(QIcon(":images/model.png"),tr("运行模式"),this);

	m_userMgrAction = new QAction(QIcon(":images/usermgr.png"),tr("用户设置"),this);
	m_stationTypeAction = new QAction(QIcon(":images/stationmgr.png"),tr("电站类别设置"),this);
	m_stationRelaAction = new QAction(QIcon(":images/stationrela.png"),tr("电站设置"),this);

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
	


	// 1 指针 2 手掌
	m_viewModelAction->setData(QVariant(QGraphicsView::ScrollHandDrag));

	connect(m_homeAction,SIGNAL(triggered()),this,SLOT(goHome()));
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
	pTimer->stop();
	NetClient::instance()->close();
}

void MainWindow::goHome()
{
	this->hide();
	HomeWindow::instance()->show();
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


	// 加载设备状态数据
	PBNS::DevStateMsg_Request req;
	req.set_saveid(1);
	req.set_stationid(1);
	string reqstr;
	req.SerializeToString(&reqstr);
	NetClient::instance()->sendData(CMD_DEV_STATE,reqstr.c_str(),reqstr.length());
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

void MainWindow::initStatusBar()
{
	m_pConnLabel = new QLabel(this);
	this->statusBar()->addPermanentWidget(m_pConnLabel);
	//无边框设置
	statusBar()->setStyleSheet(QString("QStatusBar::item{border: 0px}"));

}

void MainWindow::recvdata(int msgtype,const char* msg,int msglength)
{
	qDebug()<<msgtype;
	qDebug()<<msg;
	qDebug()<<msglength;
	
	switch (msgtype)
	{
	case CMD_DEV_STATE:
		m_sence->showDevState(msg,msglength);
		break;
	}

}
//初始化网络
void MainWindow::initNet()
{
	//读取配置文件信息
	if(!readIpPort())
	{
		QMessageBox::information(this,tr("读取配置文件提示:"),tr("请检查配置文件内容是否正确或文件是否存在!"));
		//return;
	}

	//建立网络连接类与当前类的信号与槽的关系
	connect(NetClient::instance(),SIGNAL(connected()),this,SLOT(connected()));
	connect(NetClient::instance(),SIGNAL(disconnected()),this,SLOT(disconnected()));

	connect(NetClient::instance(),SIGNAL(recvdata(int,const char* ,int)),this,SLOT(recvdata(int,const char* ,int)));

	//定时器
	pTimer = new QTimer(this);
	connect(pTimer,SIGNAL(timeout()),this,SLOT(checkConnect()));

	//连接指定IP和端口的服务器
	bNetflag = NetClient::instance()->connectToServer(m_IP,m_Port.toUInt());

	//当客户端连接上启动定时器
	//pTimer->start(5000);

}

//读取连接服务器IP和端口
bool MainWindow::readIpPort()
{
	//从指定的路径下读取INI文件对应节点信息:连接IP和指定端口
	QSettings syncini(QCoreApplication::applicationDirPath()+"/client.conf",QSettings::IniFormat);
	QString strIP = syncini.value("NETCLIENT/IP").toString();
	QString strPort = syncini.value("NETCLIENT/PORT").toString();

	//赋值
	m_IP = strIP;
	m_Port = strPort;

	//判断读取INI配置文件是否正确
	if(m_IP=="" || m_Port=="")
	{
		QString warning = "================= 请检查配置文件内容是否正确或文件是否存在! ===================!";
		qWarning(warning.toLatin1().data());

		return false;
	}

	return true;
}

void MainWindow::checkConnect()
{
	qDebug()<<"============检查与服务器连接============";

	//定时检测连接服务端是否正常，不正常进行重连
	if(!bNetflag)
	{
		QPalette pe;
		pe.setColor(QPalette::WindowText,Qt::red);
		m_pConnLabel->setPalette(pe);
		m_pConnLabel->setText(tr("通信状态:%1连接失败").arg(m_IP));

		//重新连接指定IP和端口的服务器
		bNetflag = NetClient::instance()->connectToServer(m_IP,m_Port.toUInt());
	}

	return;
}

void MainWindow::connected()
{
	bNetflag = true;
	QPalette pa;
	pa.setColor(QPalette::WindowText,Qt::green);
	m_pConnLabel->setPalette(pa);
	m_pConnLabel->setText(tr("通信状态:%1连接成功").arg(m_IP));

	pTimer->stop();
}

void MainWindow::disconnected()
{
	pTimer->start(1);

	bNetflag = false;
	QPalette pe;
	pe.setColor(QPalette::WindowText,Qt::red);
	m_pConnLabel->setPalette(pe);
	m_pConnLabel->setText(tr("通信状态:%1连接失败").arg(m_IP));

}


//关闭响应事件
void MainWindow::closeEvent(QCloseEvent *event)
{
	//pStaTypeDockWidget->close();
	return;
}