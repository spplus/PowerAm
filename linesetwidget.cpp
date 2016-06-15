#include "linesetwidget.h"
#include "comutil.h"
LineSetWidget::LineSetWidget(QWidget *parent /* = 0 */)
	:QDialog(parent)
{
	initUi();
	initConnections();
	initStationList();
}

void LineSetWidget::initUi()
{
	QVBoxLayout*		vbox = new QVBoxLayout;
	QHBoxLayout*		hbox1 = new QHBoxLayout;
	QHBoxLayout*		hbox2 = new QHBoxLayout;
	QHBoxLayout*		hbox3 = new QHBoxLayout;

	QLabel* plabelOne = new QLabel;
	plabelOne->setText(tr("站点1"));
	m_stationOneBox = new QComboBox;
	hbox1->addStretch();
	hbox1->addWidget(plabelOne);
	hbox1->addWidget(m_stationOneBox);
	hbox1->addStretch();

	QLabel* plabelOhr = new QLabel;
	plabelOhr->setText(tr("站点2"));
	m_stationOhrBox = new QComboBox;
	hbox2->addStretch();
	hbox2->addWidget(plabelOhr);
	hbox2->addWidget(m_stationOhrBox);
	hbox2->addStretch();

	m_okBtn = new QPushButton(tr("确定"));
	m_cancelBtn = new QPushButton(tr("取消"));

	hbox3->addStretch();
	hbox3->addWidget(m_okBtn);
	hbox3->addWidget(m_cancelBtn);
	hbox3->addStretch();

	vbox->addLayout(hbox1);
	vbox->addLayout(hbox2);
	vbox->addLayout(hbox3);

	setLayout(vbox);

	setFixedSize(500,300);
	setWindowFlags(Qt::WindowCloseButtonHint);
	setWindowTitle(tr("设置进出线"));
}

void LineSetWidget::initConnections()
{
	connect(m_okBtn,SIGNAL(pressed()),this,SLOT(onOk()));
	connect(m_cancelBtn,SIGNAL(pressed()),this,SLOT(onCancel()));
	connect(m_stationOneBox,SIGNAL(currentIndexChanged(int)),this,SLOT(oneItemChanged(int)));
	connect(m_stationOhrBox,SIGNAL(currentIndexChanged(int)),this,SLOT(ohrItemChanged(int)));
}

void LineSetWidget::initStationList()
{
	
	vector<Station_S> slist = ComUtil::instance()->getStationMgrList();
	for (int i = 0;i<slist.size();i++)
	{
		Station_S station = slist.at(i);
		m_stationOneBox->insertItem(i,station.curname,station.cimid);
		m_stationOhrBox->insertItem(i,station.curname,station.cimid);
	}
}

void LineSetWidget::onOk()
{
	this->accept();
}

void LineSetWidget::onCancel()
{
	this->reject();
}

void LineSetWidget::oneItemChanged(int index)
{
	m_oneCim = m_stationOneBox->itemData(index).toString();
}

void LineSetWidget::ohrItemChanged(int index)
{
	m_ohrCim = m_stationOhrBox->itemData(index).toString();
}

QString LineSetWidget::getOhrCim()
{
	return m_ohrCim;
}

QString LineSetWidget::getOneCim()
{
	return m_oneCim;
}