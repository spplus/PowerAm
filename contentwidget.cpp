#include "contentwidget.h"

// ���屣�水ť���Եļ�ֵ
#define KEY_STATION		"STATION"

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


void ContentWidget::loadData(PBNS::StationTypeMsg_Response& res,int tpid)
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

	// ���Ҹ�����վ���б�
	PBNS::StationTypeBean typebean;
	for (int i = 0;i<res.typelist_size();i++)
	{
		typebean = res.typelist(i);
		if (typebean.id() == tpid)
		{
			break;
		}
	}

	int idx = 0;
	int count =typebean.stationlist_size();
	int row = count/COL;
	if (count%COL>0)
	{
		row++;
	}
	// ���ذ�ť

	for (int i = 1;i<=row;i++)
	{
		for (int j = 1;j<=COL;j++)
		{
			
			if (idx>= count)
			{
				break;
			}

			PBNS::StationBean bean = typebean.stationlist(idx);

			QPushButton* btn = new QPushButton(bean.currentname().c_str());
			btn->setObjectName("station");

			QVariant val;
			val.setValue(bean);

			// ����վ��bean
			btn->setProperty(KEY_STATION,val);
			//btn->setProperty("sname",btn->text());
			
			m_gbox->addWidget(btn,i,j,1,1);
			connect(btn,SIGNAL(pressed()),this,SLOT(btnPressed()));

			idx++;

		}
	}

	setWidget(m_widget);
}


void ContentWidget::btnPressed()
{
	QObject *sd =  QObject::sender ();
	QVariant val = sd->property(KEY_STATION);
	PBNS::StationBean bean = val.value<PBNS::StationBean>();

	// ���ʹ��ļ��ź�
	emit openfile(bean);
}