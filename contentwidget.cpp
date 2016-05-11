#include "contentwidget.h"


ContentWidget::ContentWidget(QWidget* parent/* = NULL */)
	:QScrollArea(parent)
{
	m_widget = NULL;
	m_gbox = NULL;
	setFrameShape(QScrollArea::NoFrame);

}

ContentWidget::~ContentWidget()
{

}


void ContentWidget::loadData(PBNS::StationListMsg_Response& res)
{
	m_stationList = res;

	if (m_gbox != NULL)
	{
		delete m_gbox;
		m_gbox = NULL;
	}

	if (m_widget != NULL)
	{
		delete m_widget;
		m_widget = NULL;
	}

	QWidget* m_widget = new QWidget; 
	m_gbox = new QGridLayout;
	m_widget->setLayout(m_gbox);

	int idx = 0;
	int count =res.stationlist_size();
	int row = count/COL;
	if (count%COL>0)
	{
		row++;
	}
	// 加载按钮

	for (int i = 1;i<=row;i++)
	{
		for (int j = 1;j<=COL;j++)
		{
			
			if (idx>= count)
			{
				break;
			}

			PBNS::StationBean bean = res.stationlist(idx);

			QPushButton* btn = new QPushButton(bean.currentname().c_str());
			btn->setObjectName("station");

			// 设置文件名称
			btn->setProperty("fname",bean.path().c_str());
			btn->setProperty("sname",btn->text());
			m_gbox->addWidget(btn,i,j,1,1);
			connect(btn,SIGNAL(pressed()),this,SLOT(btnPressed()));

			idx++;

			// 保持node
			ComUtil::instance()->saveStationList(makeNode(bean,idx==count?true:false),bean.categoryid());
		}
	}

	setWidget(m_widget);
}

TreeNode* ContentWidget::makeNode(PBNS::StationBean & bean,bool islast)
{
	TreeNode * node = new TreeNode;
	node->theLast = islast;
	node->filePath = bean.path().c_str();
	node->label = bean.currentname().c_str();
	node->level = 2;
	node->nodeId = bean.id();
	return node;
}

void ContentWidget::btnPressed()
{
	QObject *sd =  QObject::sender ();
	QString fname  = sd->property("fname").toString();

	QString sname = sd->property("sname").toString();

	// 发送打开文件信号
	emit openfile(fname,sname);
}