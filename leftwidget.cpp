﻿#include "leftwidget.h"
#include "define.h"

LeftWidget::LeftWidget(QWidget* parent/* =NULL */)
	:QWidget(parent)
{
	initUi();
}

LeftWidget::~LeftWidget()
{

}

void LeftWidget::initUi()
{
	QVBoxLayout* vbox = new QVBoxLayout;
	m_list = new QListWidget;
	m_list->setSpacing(2);
	m_list->setFrameShape(QListWidget::NoFrame);
	vbox->addWidget(m_list);
	setLayout(vbox);
	vbox->setMargin(0);

	connect(m_list,SIGNAL(currentItemChanged(QListWidgetItem * ,QListWidgetItem * )),this,SLOT(currentItemChange(QListWidgetItem * , QListWidgetItem * )));
}

void LeftWidget::loadData(PBNS::StationTypeMsg_Response& res)
{
	m_list->clear();

	m_typeList = res;
	for (int i = 0;i<res.typelist_size();i++)
	{
		PBNS::StationTypeBean bean = res.typelist(i);
		QListWidgetItem* item = new QListWidgetItem;

		item->setText(bean.name().c_str());
		item->setData(Qt::UserRole,bean.id());
		item->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
		m_list->addItem(item);
		if (i == 0)
		{
			item->setSelected(true);
		}
	}
	// 加载第一个类别的站点
	if (res.typelist_size() > 0)
	{
		PBNS::StationTypeBean bean = res.typelist(0);
		emit loadStations(bean.id(),bean.name().c_str());
	}
}

TreeNode* LeftWidget::makeNode(PBNS::StationTypeBean & bean,int count)
{
	TreeNode* node = new TreeNode;
	node->nodeId = bean.id();
	node->count = count;
	node->label = bean.name().c_str();
	return node;
}

void LeftWidget::currentItemChange(QListWidgetItem * current, QListWidgetItem * previous)
{
	// 获取当前选中的站点类型
	int id = current->data(Qt::UserRole).toInt();
	QString tname = current->text();
	emit loadStations(id,tname);
}