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
	pwelcome->setText("��ӭ��,");
	pwelcome->setStyleSheet("color:white");

	QLabel* puser = new QLabel;
	puser->setText(m_userName);
	puser->setStyleSheet("color:yellow");

	QToolButton* logout= new QToolButton;
	logout->setAutoRaise(true);
	logout->setObjectName("logout");
	
	logout->setText("ע��");
	hbox->addWidget(ptime);
	hbox->addStretch(3);
	hbox->addWidget(pwelcome);
	hbox->addWidget(puser);
	hbox->addWidget(logout);

	QHBoxLayout* hbox1 = new QHBoxLayout;

	QLabel *title = new QLabel;
	title->setObjectName("titleText");
	title->setText("����һ�廯ϵͳ");
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