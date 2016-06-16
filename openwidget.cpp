#include "openwidget.h"
OpenWidget::OpenWidget(QWidget* parent /* = NULL */)
	:QDialog(parent)
{
	m_saveList = NULL;
	initUi();
	initTable();
	initData();
}

void OpenWidget::initUi()
{
	QVBoxLayout* vbox = new QVBoxLayout;
	QHBoxLayout* hbox = new QHBoxLayout;

	m_saveList = new QTableWidget;
	hbox->addWidget(m_saveList);
	QHBoxLayout* hbox2 = new QHBoxLayout;
	m_okBtn = new QPushButton(tr("ȷ��"));
	m_cancelBtn = new QPushButton(tr("ȡ��"));
	hbox2->addWidget(m_okBtn);
	hbox2->addWidget(m_cancelBtn);

	vbox->addLayout(hbox);
	vbox->addLayout(hbox2);

	setLayout(vbox);
	setFixedSize(500,400);
	setWindowFlags(Qt::WindowCloseButtonHint);
	setWindowTitle(tr("ѡ��浵"));

}

void OpenWidget::initSlots()
{
	connect(m_okBtn,SIGNAL(pressed()),this,SLOT(accept()));
	connect(m_okBtn,SIGNAL(pressed()),this,SLOT(reject()));
	connect(m_saveList,SIGNAL(itemDoubleClicked ( QTableWidgetItem *)),this,SLOT(itemDoubleClicked ( QTableWidgetItem *)));
}

void OpenWidget::initData()
{
	m_saveList->clearContents();	

}

void OpenWidget::initTable()
{
	if (m_saveList == NULL)
	{
		return;
	}
	QStringList heads;
	heads.append("���");
	heads.append("�浵����");
	heads.append("�浵ʱ��");
	m_saveList->setColumnCount(3);
	m_saveList->setHorizontalHeaderLabels(heads);
}

void OpenWidget::itemDoubleClicked( QTableWidgetItem * item )
{
	m_saveId = item->data(Qt::UserRole).toInt();
}

