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
	pwelcome->setText("��ӭ��,");
	pwelcome->setStyleSheet("color:white");

	m_userName = new QLabel;
	m_userName->setStyleSheet("color:yellow");

	m_logout= new QToolButton;
	m_logout->setAutoRaise(true);
	m_logout->setObjectName("logout");
	
	m_logout->setText(tr("ע��"));
	m_logout->setIcon(QIcon(":images/logout.png"));
	m_logout->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
	m_setting = new QToolButton();

	m_setting->setText(tr("����"));
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
	hbox->addWidget(m_setting);
	hbox->addWidget(m_logout);

	QHBoxLayout* hbox1 = new QHBoxLayout;

	QLabel *title = new QLabel;
	title->setObjectName("titleText");
	title->setText(ComUtil::instance()->getSysName());
	title->setStyleSheet("font-size:30px;color:white;font-family:΢���ź�,����;font-weight:bold;");

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
}

void TitleWidget::createMenu()
{
	m_menu = new QMenu;
	m_userAction = new QAction(QIcon(":images/usermgr.png"),tr("�û�����"),this);
	m_typeAction= new QAction(QIcon(":images/typemgr.png"),tr("������"),this);
	m_stationAction= new QAction(QIcon(":images/stationmgr.png"),tr("վ��༭"),this);
	m_roleAction= new QAction(QIcon(":images/rolemgr.png"),tr("����༭"),this);
	m_menu->addAction(m_userAction);
	m_menu->addAction(m_typeAction);
	m_menu->addAction(m_roleAction);
	m_menu->addAction(m_stationAction);

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
		week = tr("����һ");
		break;
	case Qt::Tuesday:
		week = tr("���ڶ�");
		break;
	case Qt::Wednesday:
		week = tr("������");
		break;
	case Qt::Thursday:
		week = tr("������");
		break;
	case Qt::Friday:
		week = tr("������");
		break;
	case Qt::Saturday:
		week = tr("������");
		break;
	case Qt::Sunday:
		week = tr("������");
		break;
	default:
		week = tr("UN KNOWN");
		break;
	}
	return tr("������:%1��%2��%3�� %4").arg(year).arg(month).arg(day).arg(week);
}

void TitleWidget::userMgr()
{
	m_pUserMgrdlg = new UserMgrdlg(this);

	//�����û��б�
	m_pUserMgrdlg->reqUserList();

	m_pUserMgrdlg->setWindowIcon(QIcon(":images/usermgr.png"));
	m_pUserMgrdlg->setWindowTitle("�û�����");

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

}

void TitleWidget::roleMgr()
{

}