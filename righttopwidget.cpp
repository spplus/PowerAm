#include "righttopwidget.h"

RightTopWidget::RightTopWidget(QWidget* parent /* = NULL */)
{
	m_oppath = tr("Ê×Ò³");
	initUi();
}

void RightTopWidget::setOppath(QString path)
{
	QString temp = tr("Ê×Ò³ > %1").arg(path);
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
	
	m_label = new QLabel;
	m_label->setText(m_oppath);
	hbox1->addSpacing(40);
	hbox1->addWidget(m_label);
	hbox1->addStretch();

	setStyleSheet("background:url(:images/righttop.png);");
	setLayout(hbox1);
}