#include "titlewidget.h"

TitleWidget::TitleWidget(QString username,QWidget *parent /* = 0 */)
	:QMainWindow(parent)
{
	m_userName = username;
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

	QLabel* puser = new QLabel;
	puser->setText(m_userName);
	puser->setStyleSheet("color:yellow");

	QToolButton* logout= new QToolButton;
	logout->setAutoRaise(true);
	logout->setObjectName("logout");
	
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

	setCentralWidget(titleWidget);

	connect(logout,SIGNAL(pressed()),this,SIGNAL(logout()));
}

void TitleWidget::setUserName(QString userName)
{
	m_userName = userName;
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