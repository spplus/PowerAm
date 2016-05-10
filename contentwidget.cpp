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


void ContentWidget::loadData(int row,int col)
{

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

	// 加载按钮
	for (int i = 0;i<row;i++)
	{
		for (int j = 0;j<col;j++)
		{
			QPushButton* btn = new QPushButton(tr("站点%1").arg((i+1)*(j+1)));
			btn->setObjectName("station");
			// 设置文件名称
			btn->setProperty("fname",QVariant(i*j));
			btn->setProperty("sname",btn->text());
			m_gbox->addWidget(btn,i,j,1,1);
			connect(btn,SIGNAL(pressed()),this,SLOT(btnPressed()));
		}
	}

	setWidget(m_widget);
}

void ContentWidget::btnPressed()
{
	QObject *sd =  QObject::sender ();
	QString fname  = sd->property("fname").toString();

	QString sname = sd->property("sname").toString();

	// 发送打开文件信号
	emit openfile(fname,sname);
}