#include "rulecheckwidget.h"

RuleCheckWidget::RuleCheckWidget(QWidget* parent /* = NULL */)
	:QDialog(parent)
{
	m_saveList = NULL;
	initUi();
	initTable();
	initSlots();
}

void RuleCheckWidget::initUi()
{
	QVBoxLayout* vbox = new QVBoxLayout;
	QHBoxLayout* hbox = new QHBoxLayout;

	m_saveList = new QTableWidget;
	m_saveList->setEditTriggers(QAbstractItemView::NoEditTriggers);
	m_saveList->setSelectionBehavior(QAbstractItemView::SelectRows);
	m_saveList->setSelectionMode(QAbstractItemView::SingleSelection);

	hbox->addWidget(m_saveList);
	QHBoxLayout* hbox2 = new QHBoxLayout;
	m_okBtn = new QPushButton(tr("ִ��"));
	m_cancelBtn = new QPushButton(tr("ȡ��"));

	hbox2->addStretch();
	hbox2->addWidget(m_okBtn);
	hbox2->addWidget(m_cancelBtn);
	hbox2->addStretch();

	vbox->addLayout(hbox);
	vbox->addLayout(hbox2);

	setLayout(vbox);
	setFixedSize(450,350);
	setWindowFlags(Qt::WindowCloseButtonHint);
	setWindowTitle(tr("����У��"));

}

void RuleCheckWidget::initSlots()
{
	connect(m_okBtn,SIGNAL(pressed()),this,SLOT(accept()));
	connect(m_cancelBtn,SIGNAL(pressed()),this,SLOT(reject()));
}


void RuleCheckWidget::initData()
{
	m_saveList->clearContents();	
	int row = m_dataList.rulelist_size();
	m_saveList->setRowCount(row);

	for (int i=0;i<m_dataList.rulelist_size();i++)
	{
		PBNS::RuleBean bean = m_dataList.rulelist(i);
		
		QTableWidgetItem *itemName = new QTableWidgetItem;
		itemName->setText(bean.name().c_str());
		itemName->setData(Qt::UserRole,bean.id());

		QTableWidgetItem *itemLevel = new QTableWidgetItem;
		itemLevel->setText(bean.alarmlevel().c_str());
		itemLevel->setData(Qt::UserRole,bean.id());

		QTableWidgetItem *itemDesc = new QTableWidgetItem;
		itemDesc->setText(bean.description().c_str());
		itemDesc->setData(Qt::UserRole,bean.id());

		m_saveList->setItem(i,0,itemName);
		m_saveList->setItem(i,1,itemLevel);
		m_saveList->setItem(i,2,itemDesc);
	}
	m_saveList->resizeColumnsToContents();
}

void RuleCheckWidget::initTable()
{
	if (m_saveList == NULL)
	{
		return;
	}
	QStringList heads;
	heads.append("����");
	heads.append("�澯�ȼ�");
	heads.append("����");
	m_saveList->setColumnCount(3);
	m_saveList->setHorizontalHeaderLabels(heads);
}

void RuleCheckWidget::setData(PBNS::OprationMsg_Response &res)
{
	m_dataList = res;
	initData();
}