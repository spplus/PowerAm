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


HomeWindow::HomeWindow(QWidget *parent /* = 0 */)
	:QMainWindow(parent)
{
	initUi();

	loadData();
}

HomeWindow::~HomeWindow()
{

}

void HomeWindow::initUi()
{
	QVBoxLayout* vmain = new QVBoxLayout;
	QHBoxLayout* titleHbox = new QHBoxLayout;
	m_titleWidget = new TitleWidget("管理员");
	
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

void HomeWindow::loadData()
{
	m_leftWidget->loadData();
	m_contentWidget->loadData();
}

void HomeWindow::initConnections()
{
	connect(m_leftWidget,SIGNAL(loadStations(int)),this,SLOT(loadStationsById(int)));
	connect(m_titleWidget,SIGNAL(logout()),this,SLOT(logout()));
}

void HomeWindow::logout()
{

}

void HomeWindow::loadStationsById(int id)
{

}

QWidget* HomeWindow::initTitleWidget()
{
	QWidget* titleWidget = new TitleWidget("管理员");

	/*

	titleWidget->setObjectName("title");
	titleWidget->setStyleSheet("#title { border-image: url(:/images/title.png); }#title * { border-image:url(); } ");

	QVBoxLayout* vbox = new QVBoxLayout;
	QHBoxLayout* hbox = new QHBoxLayout;

	QLabel* ptime = new QLabel;
	ptime->setText("今天是:2016年5月7日 星期六");
	ptime->setStyleSheet("color:#006A6A");
	QLabel* pwelcome = new QLabel;
	pwelcome->setText("欢迎您,");
	pwelcome->setStyleSheet("color:white");
	QLabel* puser = new QLabel;
	puser->setText("管理员");
	puser->setStyleSheet("color:yellow");
	
	QToolButton* logout= new QToolButton;
	logout->setAutoRaise(true);
	
	logout->setStyleSheet("QToolButton{color:white} :hover{color: rgb(224, 0, 0);border-style: inset;}");
	logout->setText("注销");
	hbox->addWidget(ptime);
	hbox->addStretch(3);
	hbox->addWidget(pwelcome);
	hbox->addWidget(puser);
	hbox->addWidget(logout);

	QHBoxLayout* hbox1 = new QHBoxLayout;

	QLabel *title = new QLabel;
	title->setObjectName("titleText");
	title->setText("调控一体化系统");
	title->setStyleSheet("font-size:30px;color:white;font-family:微软雅黑,宋体;font-weight:bold;");

	QLabel* logo = new QLabel;
	logo->setPixmap(QPixmap(":/images/logo.png"));

	hbox1->addWidget(logo);
	hbox1->addSpacing(10);
	hbox1->addWidget(title);
	hbox1->addStretch();
	vbox->addStretch();
	vbox->addLayout(hbox1);
	vbox->addStretch();
	vbox->addLayout(hbox);

	titleWidget->setLayout(vbox);
	*/
	return titleWidget;
}

QWidget* HomeWindow::initLeftWidget()
{
	QWidget* leftWidget = new QWidget;
	QVBoxLayout* vbox = new QVBoxLayout;
	QListWidget* list = new QListWidget;
	list->setSpacing(2);
	
	list->setFrameShape(QListWidget::NoFrame);
	for (int i = 0;i<10;i++)
	{
		QListWidgetItem* item = new QListWidgetItem;

		item->setText("测试测试");
		
		item->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
		list->addItem(item);
	}
	vbox->addWidget(list);
	leftWidget->setLayout(vbox);
	vbox->setMargin(0);

	return leftWidget;
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
QWidget* HomeWindow::initRightBottomWidget()
{
	QWidget* rightBottomWidget = new QWidget;

	QGridLayout* gbox = new QGridLayout;

	// 加载按钮
	for (int i = 0;i<10;i++)
	{
		for (int j = 0;j<5;j++)
		{
			QPushButton* btn = new QPushButton("站点");
			gbox->addWidget(btn,i,j,1,1);
		}
	}
	
	rightBottomWidget->setLayout(gbox);

	QScrollArea * scrollArea = new QScrollArea;
	scrollArea->setWidget(rightBottomWidget);
	scrollArea->setFrameShape(QScrollArea::NoFrame);

	return scrollArea;
}
QWidget* HomeWindow::initRightWidet()
{
	QWidget* rightWidget = new QWidget;
	QVBoxLayout* vbox = new QVBoxLayout;
	QHBoxLayout* hbox1 = new QHBoxLayout;
	QHBoxLayout* hbox2 = new QHBoxLayout;
	m_contentWidget = new ContentWidget;
	hbox1->addWidget(initRightTopWidget());
	hbox2->addWidget(m_contentWidget);
	vbox->addLayout(hbox1);
	vbox->addLayout(hbox2,1);
	rightWidget->setLayout(vbox);
	return rightWidget;
}

QWidget* HomeWindow::initRightTopWidget()
{
	QWidget* rightTopWidget = new QWidget;
	QHBoxLayout* hbox1 = new QHBoxLayout;
	QLabel* home = new QLabel;
	home->setPixmap(QPixmap(":images/home.png"));
	QLabel* nav = new QLabel;
	nav->setText("首页 > 傲城站");
	hbox1->addSpacing(40);
	hbox1->addWidget(nav);
	hbox1->addStretch();

	rightTopWidget->setStyleSheet("background:url(:images/righttop.png);");
	rightTopWidget->setLayout(hbox1);
	//hbox1->setMargin(0);
	//hbox1->setContentsMargins(10,5,0,5);
	
	return rightTopWidget;
}

QWidget* HomeWindow::initBottomWidget()
{
	QWidget* bottomWidget = new QWidget;
	bottomWidget->setObjectName("bottom");
	//bottomWidget->setMaximumHeight(70);
	QHBoxLayout*hbox = new QHBoxLayout;
	QLabel* sysname = new QLabel;
	sysname->setObjectName("sysname");
	sysname->setText("调控一体化防误系统");
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