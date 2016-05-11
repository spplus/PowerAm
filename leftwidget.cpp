#include "leftwidget.h"
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
	m_typeList = res;
	for (int i = 0;i<res.typelist_size();i++)
	{
		PBNS::StationTypeBean bean = res.typelist(i);
		QListWidgetItem* item = new QListWidgetItem;

		item->setText(bean.name().c_str());
		item->setData(Qt::UserRole,bean.id());
		item->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
		m_list->addItem(item);
	}
}

void LeftWidget::currentItemChange(QListWidgetItem * current, QListWidgetItem * previous)
{
	// 获取当前选中的站点类型
	int id = current->data(Qt::UserRole).toInt();
	QString tname = current->text();
	emit loadStations(id,tname);
}