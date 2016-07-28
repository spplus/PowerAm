﻿#include "openwidget.h"
OpenWidget::OpenWidget(QWidget* parent /* = NULL */)
	:QDialog(parent)
{
	m_saveList = NULL;
	initUi();
	initTable();
	initSlots();
}

void OpenWidget::initUi()
{
	QVBoxLayout* vbox = new QVBoxLayout;
	QHBoxLayout* hbox = new QHBoxLayout;

	m_saveList = new QTableWidget;
	m_saveList->setEditTriggers(QAbstractItemView::NoEditTriggers);
	m_saveList->setSelectionBehavior(QAbstractItemView::SelectRows);
	m_saveList->setSelectionMode(QAbstractItemView::SingleSelection);
	


	hbox->addWidget(m_saveList);
	QHBoxLayout* hbox2 = new QHBoxLayout;
	m_okBtn = new QPushButton(tr("确定"));
	m_cancelBtn = new QPushButton(tr("取消"));
	
	hbox2->addStretch();
	hbox2->addWidget(m_okBtn);
	hbox2->addWidget(m_cancelBtn);
	hbox2->addStretch();

	vbox->addLayout(hbox);
	vbox->addLayout(hbox2);

	setLayout(vbox);
	setFixedSize(400,350);
	setWindowFlags(Qt::WindowCloseButtonHint);
	setWindowTitle(tr("选择存档"));

}

void OpenWidget::initSlots()
{
	connect(m_okBtn,SIGNAL(pressed()),this,SLOT(onOk()));
	connect(m_cancelBtn,SIGNAL(pressed()),this,SLOT(reject()));
	connect(m_saveList,SIGNAL(itemDoubleClicked ( QTableWidgetItem *)),this,SLOT(itemDoubleClicked ( QTableWidgetItem *)));
}

void OpenWidget::onOk()
{
	QTableWidgetItem* curitem = m_saveList->currentItem();
	if(curitem != NULL)
	{
		m_saveId = curitem->data(Qt::UserRole).toInt();
		accept();
	}
	else
	{
		QMessageBox::warning(this,MSG_TITLE,"请选择一条存档记录");
	}
}

void OpenWidget::initData()
{
	m_saveList->clearContents();	
	int row = m_dataList.savelist_size();
	m_saveList->setRowCount(row);

	for (int i=0;i<m_dataList.savelist_size();i++)
	{
		PBNS::SavingBean bean = m_dataList.savelist(i);

		QTableWidgetItem *itemName = new QTableWidgetItem;
		itemName->setText(bean.name().c_str());
		itemName->setData(Qt::UserRole,bean.id());

		QTableWidgetItem *itemTime = new QTableWidgetItem;
		itemTime->setText(bean.savetime().c_str());
		itemTime->setData(Qt::UserRole,bean.id());

		m_saveList->setItem(i,0,itemName);
		m_saveList->setItem(i,1,itemTime);
	}
	m_saveList->resizeColumnsToContents();
	m_saveList->setCurrentCell(-1,-1);
}

void OpenWidget::initTable()
{
	if (m_saveList == NULL)
	{
		return;
	}
	QStringList heads;
	heads.append("存档名称");
	heads.append("存档时间");
	m_saveList->setColumnCount(2);
	m_saveList->setHorizontalHeaderLabels(heads);
	m_saveList->setSelectionBehavior(QAbstractItemView::SelectRows); 
	m_saveList->setSelectionMode(QAbstractItemView::SingleSelection);

}

void OpenWidget::itemDoubleClicked( QTableWidgetItem * item )
{
	m_saveId = item->data(Qt::UserRole).toInt();
	accept();
}

void OpenWidget::setData(PBNS::SavingListMsg_Response &res)
{
	m_dataList = res;
	initData();
}

int OpenWidget::getSaveId()
{
	return m_saveId;
}