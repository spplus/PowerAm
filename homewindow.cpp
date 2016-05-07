#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QToolButton>
#include <QWidget>
#include <QListWidget>
#include <QLabel>
#include <QApplication>
#include <QDesktopWidget>
#include <QPixmap>
#include "homewindow.h"


HomeWindow::HomeWindow(QWidget *parent /* = 0 */)
	:QMainWindow(parent)
{
	initUi();
}

HomeWindow::~HomeWindow()
{

}

void HomeWindow::initUi()
{
	QVBoxLayout* vmain = new QVBoxLayout;
	QHBoxLayout* titleHbox = new QHBoxLayout;
	titleHbox->addWidget(initTitleWidget());

	QHBoxLayout* mainHbox = new QHBoxLayout;
	QVBoxLayout* leftVbox = new QVBoxLayout;
	leftVbox->addWidget(initLeftWidget());

	QVBoxLayout* midVbox = new QVBoxLayout;
	midVbox->addWidget(initMidWidget());

	QVBoxLayout* rightVbox = new QVBoxLayout;
	rightVbox->addWidget(initRightWidet());
	mainHbox->addLayout(leftVbox);
	mainHbox->addLayout(midVbox);
	mainHbox->addLayout(rightVbox,2);
	
	mainHbox->setMargin(0);
	leftVbox->setMargin(0);
	rightVbox->setMargin(0);
	midVbox->setMargin(0);
	vmain->setMargin(0);
	titleHbox->setMargin(0);

	vmain->addLayout(titleHbox);
	vmain->addLayout(mainHbox,1);

	QWidget *centralWidget = new QWidget;
	centralWidget->setLayout(vmain);
	setCentralWidget(centralWidget);
}

QWidget* HomeWindow::initTitleWidget()
{
	QWidget* titleWidget = new QWidget;

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

	return titleWidget;
}


QWidget* HomeWindow::initLeftWidget()
{
	QWidget* leftWidget = new QWidget;
	QVBoxLayout* vbox = new QVBoxLayout;
	QListWidget* list = new QListWidget;
	for (int i = 0;i<10;i++)
	{
		QListWidgetItem* item = new QListWidgetItem;
		item->setText("测试测试");
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

QWidget* HomeWindow::initRightWidet()
{
	QWidget* rightWidget = new QWidget;
	QVBoxLayout* vbox = new QVBoxLayout;
	QHBoxLayout* hbox1 = new QHBoxLayout;

	hbox1->addWidget(initRightTopWidget());

	QHBoxLayout* hbox2 = new QHBoxLayout;
	
	vbox->addLayout(hbox1);
	vbox->addLayout(hbox2,1);

	hbox1->setMargin(0);
	vbox->setMargin(0);

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
	nav->setText("首页>傲城站");
	hbox1->addWidget(home);
	hbox1->addWidget(nav);
	hbox1->addStretch();

	rightTopWidget->setStyleSheet("background-color:qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,stop: 0 #F0F0F0, stop: 1 #8588B2);");
	rightTopWidget->setLayout(hbox1);
	//hbox1->setMargin(0);

	return rightTopWidget;
}