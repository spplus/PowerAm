#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QToolButton>
#include <QWidget>
#include <QListWidget>
#include <QStatusBar>
#include <QLabel>
#include <QPushButton>
#include <QPixmap>
#include "homewindow.h"
#include "mainwindow.h"
#include "include/commands.h"
#include "buff/msgbody.pb.h"
#include "netclient.h"

HomeWindow* HomeWindow::m_inst = NULL;

HomeWindow* HomeWindow::instance()
{
	if (m_inst == NULL)
	{
		m_inst = new HomeWindow;
	}
	return m_inst;
}

void HomeWindow::setUserName(QString username)
{
	m_titleWidget->setUserName(username);
}
HomeWindow::HomeWindow(QWidget *parent /* = 0 */)
	:QMainWindow(parent)
{
	// 初始化界面
	initUi();

	// 加载数据
	loadData();

	// 初始化信号槽
	initConnections();
}

HomeWindow::~HomeWindow()
{

}

void HomeWindow::initUi()
{
	QVBoxLayout* vmain = new QVBoxLayout;
	QHBoxLayout* titleHbox = new QHBoxLayout;
	m_titleWidget = new TitleWidget;

	titleHbox->addWidget(m_titleWidget);

	QHBoxLayout* mainHbox = new QHBoxLayout;
	QVBoxLayout* leftVbox = new QVBoxLayout;
	m_leftWidget = new LeftWidget;
	leftVbox->addWidget(m_leftWidget);

	QVBoxLayout* midVbox = new QVBoxLayout;
	midVbox->addWidget(initMidWidget());

	QVBoxLayout* rightVbox = new QVBoxLayout;
	rightVbox->addWidget(initRightWidet());
	mainHbox->addLayout(leftVbox);
	mainHbox->addLayout(midVbox);
	mainHbox->addLayout(rightVbox,1);
	
	QHBoxLayout* bottomHbox = new QHBoxLayout;
	bottomHbox->addWidget(initBottomWidget());
	//mainHbox->setMargin(0);
	//leftVbox->setMargin(0);
	//rightVbox->setMargin(0);
	//midVbox->setMargin(0);
	vmain->setMargin(0);
	//titleHbox->setMargin(0);

	vmain->addLayout(titleHbox);
	vmain->addLayout(mainHbox,1);
	vmain->addLayout(bottomHbox);

	QWidget *centralWidget = new QWidget;
	centralWidget->setLayout(vmain);
	setCentralWidget(centralWidget);
	this->showMaximized();
}

void HomeWindow::recvdata(int msgtype,const char* msg,int msglength)
{

	switch (msgtype)
	{
	case CMD_STATION_TYPE:
		{
			PBNS::StationTypeMsg_Response res;
			res.ParseFromArray(msg,msglength);
			m_leftWidget->loadData(res);
			m_stationList = res;
			ComUtil::instance()->saveStationList(res);
		}
		break;

	default:
		break;
	}
}

void HomeWindow::loadData()
{
	//m_leftWidget->loadData();
	//m_contentWidget->loadData();
}

void HomeWindow::initConnections()
{
	connect(m_leftWidget,SIGNAL(loadStations(int,QString)),this,SLOT(loadStationsById(int,QString)));
	connect(m_titleWidget,SIGNAL(logout()),this,SLOT(logout()));
	connect(m_contentWidget,SIGNAL(openfile(QString,QString)),this,SLOT(openFile(QString,QString)));
}


void HomeWindow::openFile(QString fname,QString sname)
{
	m_rightTopWidget->setStationName(sname);

	// 显示电路图窗口，隐藏当前窗口
	this->hide();

	MainWindow::instance()->show();
}

void HomeWindow::logout()
{

}

void HomeWindow::loadStationsById(int id,QString tname)
{
	// 按类别加载站点
	m_contentWidget->loadData(m_stationList,id);

	// 设置操作路径
	m_rightTopWidget->setOppath(tname);
}


QWidget* HomeWindow::initMidWidget()
{
	QWidget* midWidget = new QWidget;
	midWidget->setMaximumWidth(4);
	
	QHBoxLayout* hbox = new QHBoxLayout;
	midWidget->setStyleSheet("background-color: #006A6A;color: #ffffff;");
	midWidget->setLayout(hbox);
	//hbox->setMargin(0);

	return midWidget;
}

QWidget* HomeWindow::initRightWidet()
{
	QWidget* rightWidget = new QWidget;
	QVBoxLayout* vbox = new QVBoxLayout;
	QHBoxLayout* hbox1 = new QHBoxLayout;
	QHBoxLayout* hbox2 = new QHBoxLayout;
	m_contentWidget = new ContentWidget;
	m_rightTopWidget = new RightTopWidget;

	hbox1->addWidget(m_rightTopWidget);
	hbox2->addWidget(m_contentWidget);

	vbox->addLayout(hbox1);
	vbox->addLayout(hbox2,1);
	rightWidget->setLayout(vbox);
	return rightWidget;
}


QWidget* HomeWindow::initBottomWidget()
{
	QWidget* bottomWidget = new QWidget;
	bottomWidget->setObjectName("bottom");
	QHBoxLayout*hbox = new QHBoxLayout;
	QLabel* sysname = new QLabel;
	sysname->setObjectName("sysname");
	sysname->setText(ComUtil::instance()->getSysName());
	sysname->setStyleSheet("color:#006A6A;font-size:13px;");

	hbox->addStretch();
	hbox->addWidget(sysname);
	hbox->addStretch();
	hbox->setContentsMargins(0,5,0,5);
	//bottomWidget->setStyleSheet("#bottom {background-color:qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,stop: 0 #F0F0F0, stop: 1 #8588B2)}");
	bottomWidget->setStyleSheet("#bottom {background:url(:images/bottom.png);}");
	bottomWidget->setLayout(hbox);
	return bottomWidget;
}