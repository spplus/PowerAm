#include "titlewidget.h"
#include "comutil.h"

TitleWidget::TitleWidget(QWidget *parent /* = 0 */)
	:QMainWindow(parent)
{
	initUi();
}

TitleWidget::~TitleWidget()
{

}

void TitleWidget::initUi()
{
	QWidget* titleWidget = new QWidget;
	titleWidget->setObjectName("title");

	QVBoxLayout* vbox = new QVBoxLayout;
	QHBoxLayout* hbox = new QHBoxLayout;

	QLabel* ptime = new QLabel;
	ptime->setText(getDate());
	
	ptime->setStyleSheet("color:#006A6A");
	QLabel* pwelcome = new QLabel;
	pwelcome->setText("欢迎您,");
	pwelcome->setStyleSheet("color:white");

	m_userName = new QLabel;
	m_userName->setStyleSheet("color:yellow");

	m_logout= new QToolButton;
	m_logout->setAutoRaise(true);
	m_logout->setObjectName("logout");
	
	m_logout->setText(tr("注销"));
	m_logout->setIcon(QIcon(":images/logout.png"));
	m_logout->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

	m_userpwd = new QToolButton;
	m_userpwd->setAutoRaise(true);
	m_userpwd->setObjectName("logout");
	m_userpwd->setText(tr("密码重置"));
	m_userpwd->setIcon(QIcon(":images/typemgr.png"));
	m_userpwd->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

	m_setting = new QToolButton();

	m_setting->setText(tr("设置"));
	m_setting->setObjectName("logout");
	m_setting->setAutoRaise(true);
	
	m_setting->setIcon(QIcon(":images/set_system.png"));
	m_setting->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

	createMenu();
	m_setting->setMenu(m_menu);
	m_setting->setPopupMode(QToolButton::InstantPopup);

	hbox->addWidget(ptime);
	hbox->addStretch(3);
	hbox->addWidget(pwelcome);
	hbox->addWidget(m_userName);

	//只有超管角色显示设置项
	if (UserLogindlg::instance()->getLoginRoleId() == 1)
	{
		hbox->addWidget(m_setting);
	}
	else
	{
		hbox->addWidget(m_userpwd);
	}
	
	hbox->addWidget(m_logout);

	QHBoxLayout* hbox1 = new QHBoxLayout;

	QLabel *title = new QLabel;
	title->setObjectName("titleText");
	title->setText(ComUtil::instance()->getSysName());
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

	setCentralWidget(titleWidget);

	connect(m_logout,SIGNAL(pressed()),this,SIGNAL(logout()));
	connect(m_userpwd,SIGNAL(pressed()),this,SLOT(passwdMgr()));
}

void TitleWidget::createMenu()
{
	m_menu = new QMenu;
	m_userAction = new QAction(QIcon(":images/usermgr.png"),tr("用户管理"),this);
	m_typeAction= new QAction(QIcon(":images/typemgr.png"),tr("类别管理"),this);
	m_stationAction= new QAction(QIcon(":images/stationmgr.png"),tr("站点管理"),this);
	m_roleAction= new QAction(QIcon(":images/rolemgr.png"),tr("规则编辑"),this);
	m_menu->addAction(m_userAction);
	//m_menu->addAction(m_typeAction);
	m_menu->addAction(m_stationAction);
	m_menu->addAction(m_roleAction);

	connect(m_userAction,SIGNAL(triggered()),this,SLOT(userMgr()));
	connect(m_typeAction,SIGNAL(triggered()),this,SLOT(typeMgr()));
	connect(m_stationAction,SIGNAL(triggered()),this,SLOT(stationMgr()));
	connect(m_roleAction,SIGNAL(triggered()),this,SLOT(roleMgr()));

}

void TitleWidget::setUserName(QString userName)
{
	m_userName->setText(userName);
}

QString TitleWidget::getDate()
{
	QDate date = QDate::currentDate();
	int year,month,day;
	QString week;
	date.getDate(&year,&month,&day);
	
	switch (date.dayOfWeek())
	{
	case Qt::Monday:
		week = tr("星期一");
		break;
	case Qt::Tuesday:
		week = tr("星期二");
		break;
	case Qt::Wednesday:
		week = tr("星期三");
		break;
	case Qt::Thursday:
		week = tr("星期四");
		break;
	case Qt::Friday:
		week = tr("星期五");
		break;
	case Qt::Saturday:
		week = tr("星期六");
		break;
	case Qt::Sunday:
		week = tr("星期天");
		break;
	default:
		week = tr("UN KNOWN");
		break;
	}
	return tr("今天是:%1年%2月%3日 %4").arg(year).arg(month).arg(day).arg(week);
}

void TitleWidget::userMgr()
{
	m_pUserMgrdlg = new UserMgrdlg(this);

	//请求用户列表
	m_pUserMgrdlg->reqUserList();

	m_pUserMgrdlg->setWindowIcon(QIcon(":images/usermgr.png"));
	m_pUserMgrdlg->setWindowTitle("用户管理");

	m_pUserMgrdlg->exec();

	delete m_pUserMgrdlg;

	return;
}

void TitleWidget::retUserMgr(int msgtype,const char* msg,int msglength)
{
	m_pUserMgrdlg->retUserManager(msgtype,msg);
	return;
}

void TitleWidget::typeMgr()
{

}

void TitleWidget::stationMgr()
{
	m_pStationMgrdlg = new StationMgr(this);

	m_pStationMgrdlg->setWindowIcon(QIcon(":images/stationmgr.png"));
	m_pStationMgrdlg->setWindowTitle("厂站管理");

	m_pStationMgrdlg->exec();

	delete m_pStationMgrdlg;

	return;
}

void TitleWidget::retStationMgr(int msgtype,const char* msg)
{
	switch (msgtype)
	{
	case CMD_STATION_MANAGER:
		m_pStationMgrdlg->retStationMgr(msgtype,msg);
		break;
	case CMD_STATION_LIST:

		break;

	}
}

void TitleWidget::passwdMgr()
{
	m_pUserPwddlg = new UserPasswdReset(this);
	m_pUserPwddlg->setWindowIcon(QIcon(":images/typemgr.png"));
	m_pUserPwddlg->setWindowTitle("密码重置");

	m_pUserPwddlg->exec();

	delete m_pUserPwddlg;

	return;
}

void TitleWidget::retpasswdMgr(int msgtype,const char* msg)
{
	m_pUserPwddlg->retpasswdMgr(msgtype,msg);
}

void TitleWidget::roleMgr()
{

}