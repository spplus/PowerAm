#include "righttopwidget.h"

RightTopWidget::RightTopWidget(QWidget* parent /* = NULL */)
{
	m_oppath = tr("首页");
	initUi();
}

void RightTopWidget::setOppath(QString path)
{
	QString temp = tr("首页 > %1").arg(path);
	m_oppath =temp;
	m_label->setText(temp);
}

void RightTopWidget::setStationName(QString sname)
{
	QString temp = tr("%1 > %2").arg(m_oppath).arg(sname);
	m_label->setText(temp);
}

RightTopWidget::~RightTopWidget()
{

}

void RightTopWidget::initUi()
{
	QHBoxLayout* hbox1 = new QHBoxLayout;
	
	QLabel* homeLabel = new QLabel;
	homeLabel->setPixmap(QPixmap(":/images/top_home.png"));

	m_label = new QLabel;
	m_label->setText(m_oppath);
	hbox1->addWidget(homeLabel);
	hbox1->addWidget(m_label);
	hbox1->addStretch();
	m_label->setStyleSheet("font-size:15px;color:#006A6A;font-family:微软雅黑,宋体;font-weight:bold;");
	setStyleSheet("border-image:url(:images/top_bk.png);");
	setLayout(hbox1);
}