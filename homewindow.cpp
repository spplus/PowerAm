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
	setGeometry(QRect(100,100,1024,600));

	// 初始化界面
	initUi();

	// 初始化信号槽
	initConnections();

}

HomeWindow::~HomeWindow()
{

}

void HomeWindow::show()
{
	QMainWindow::show();
	activateWindow();
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
	vmain->setMargin(0);

	vmain->addLayout(titleHbox);
	vmain->addLayout(mainHbox,1);
	vmain->addLayout(bottomHbox);

	QWidget *centralWidget = new QWidget;
	centralWidget->setLayout(vmain);
	setCentralWidget(centralWidget);

	showMaximized();
}

void HomeWindow::recvdata(int msgtype,const char* msg,int msglength)
{

	switch (msgtype)
	{
	case CMD_STATION_TYPE:
		{
			PBNS::StationTypeMsg_Response res;
			res.ParseFromArray(msg,msglength);
			
			m_stationList = res;
			ComUtil::instance()->saveStationList(res);
			m_leftWidget->loadData(res);

			//厂站管理返回
			ComUtil::instance()->saveStationAndTypeList(res);
			//获取ftp路径中svg文件名称
			ComUtil::instance()->saveSvgPathName();
			//设置刷新标志
			ComUtil::instance()->setStationTypeRushflag(true);

		}
		break;
	case CMD_STATION_MANAGER:
		m_titleWidget->retStationMgr(msgtype,msg);
		break;
	case CMD_PWD_MODIFY:
		m_titleWidget->retpasswdMgr(msgtype,msg);
		break;
	case CMD_USER_MANAGER:
	case CMD_USER_ROLE:
	case CMD_USER_ADD:
	case CMD_USER_DEL:
	case CMD_USER_MODIFY:
		retUserMgr(msgtype,msg,msglength);
		break;
	default:
		break;
	}
}

void HomeWindow::retUserMgr(int msgtype,const char* msg,int msglength)
{
	m_titleWidget->retUserMgr(msgtype,msg,msglength);
}

void HomeWindow::initConnections()
{
	connect(m_leftWidget,SIGNAL(loadStations(int,QString)),this,SLOT(loadStationsById(int,QString)));
	connect(m_titleWidget,SIGNAL(logout()),this,SLOT(logout()));
	connect(m_contentWidget,SIGNAL(openfile(PBNS::StationBean&)),this,SLOT(openFile(PBNS::StationBean&)));
}


void HomeWindow::openFile(PBNS::StationBean& bean)
{
	m_rightTopWidget->setStationName(bean.currentname().c_str());

	// 显示电路图窗口，隐藏当前窗口
	this->hide();

	MainWindow::instance()->show();
	MainWindow::instance()->openFile(bean.path().c_str(),bean.cimid().c_str());
}

void HomeWindow::logout()
{
	if(QMessageBox::question(this,"系统提示","确定退出系统吗？",QMessageBox::Yes,QMessageBox::No) == QMessageBox::Yes)
	{
		exit(0);
	}
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
	bottomWidget->setStyleSheet("#bottom {border-image:url(:images/bottom.png);}");
	bottomWidget->setLayout(hbox);
	return bottomWidget;
}