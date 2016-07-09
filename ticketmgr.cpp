#include "ticketmgr.h"
#include "comutil.h"
TicketMgr::TicketMgr(QWidget* parent /* = NULL */)
{
	m_ticketTable = NULL;
	m_missionTable = NULL;
	m_crateTime = NULL;

	switch (ComUtil::instance()->getCurUserRole())
	{
	case 1://����Ա
		break;
	case 2://����
		initUi_Mission();
		break;
	case 3://�Զ���
		break;
	case 4://��ά
		initUi_Action();
		break;
	default:
		break;
	}

	initUi_Action();
	initSlots();
	initData();
}

void TicketMgr::initUi_Mission()
{
	QVBoxLayout*		vbox = new QVBoxLayout;
	QGroupBox* gbox = new QGroupBox;
	gbox->setTitle(tr("��������"));
	m_ticketTable = new QTableWidget;
	QHBoxLayout* hbox0 = new QHBoxLayout;
	hbox0->addWidget(m_ticketTable);
	gbox->setLayout(hbox0);

	QHBoxLayout* hbox1 = new QHBoxLayout;
	QLabel* doUser = new QLabel(tr("ִ����"));
	m_userBox = new QComboBox;
	QLabel* doTime = new QLabel(tr("����ʱ��"));
	m_crateTime = new QDateTimeEdit;
	m_crateTime->setCalendarPopup(true);
	hbox1->addStretch();
	hbox1->addWidget(doUser);
	hbox1->addWidget(m_userBox);
	hbox1->addWidget(doTime);
	hbox1->addWidget(m_crateTime);
	hbox1->addStretch();

	QHBoxLayout* hbox2 = new QHBoxLayout;
	m_addBtn = new QPushButton(tr("����"));
	m_saveBtn = new QPushButton(tr("����"));
	m_delBtu = new QPushButton(tr("ɾ��"));

	hbox2->addWidget(m_addBtn);
	hbox2->addWidget(m_delBtu);
	hbox2->addWidget(m_saveBtn);

	vbox->addWidget(gbox);
	vbox->addLayout(hbox1);
	vbox->addLayout(hbox2);

	connect(m_saveBtn,SIGNAL(pressed()),this,SLOT(onSave()));
	connect(m_addBtn,SIGNAL(pressed()),this,SLOT(onAdd()));
	connect(m_delBtu,SIGNAL(pressed()),this,SLOT(onDel()));

	setLayout(vbox);
}

void TicketMgr::initUi_Action()
{
	QVBoxLayout*		vbox = new QVBoxLayout;
	QGroupBox* gbox = new QGroupBox;
	gbox->setTitle(tr("��������"));
	m_ticketTable = new QTableWidget;
	QHBoxLayout* hbox0 = new QHBoxLayout;
	hbox0->addWidget(m_ticketTable);
	gbox->setLayout(hbox0);

	QHBoxLayout* hbox1 = new QHBoxLayout;
	
	QGroupBox* gbox2 = new QGroupBox;
	gbox2->setTitle(tr("����Ʊ"));
	m_missionTable = new QTableWidget;
	hbox1->addWidget(m_missionTable);
	gbox2->setLayout(hbox1);


	QHBoxLayout* hbox2 = new QHBoxLayout;
	m_createBtn = new QPushButton(tr("��������Ʊ"));
	hbox2->addWidget(gbox);
	hbox2->addWidget(m_createBtn);

	QHBoxLayout* hbox3 = new QHBoxLayout;
	hbox3->addWidget(gbox2);

	vbox->addLayout(hbox2);
	vbox->addLayout(hbox3);

	connect(m_createBtn,SIGNAL(pressed()),this,SLOT(onSaveTicket()));
	setLayout(vbox);
}

void TicketMgr::initSlots()
{
	//connect(m_saveBtn,SIGNAL(pressed()),this,SLOT(onSaveTicket()));
	//connect(m_createBtn,SIGNAL(pressed()),this,SLOT(onCreateActions()));
}

void TicketMgr::onCreateActions()
{

}

void TicketMgr::onSave()
{

}

void TicketMgr::onAdd()
{

}

void TicketMgr::onDel()
{

}


void TicketMgr::initData()
{
	if (m_missionTable != NULL)
	{
		m_missionTable->setColumnCount(8);
		m_missionTable->resizeColumnsToContents();
		QStringList missions ;
		missions<<"���"<<"��������"<<"������"<<"�໤��"<<"ֵ�ฺ����"<<"������ʼʱ��"<<"��������ʱ��"<<"��ע";
		m_missionTable->setHorizontalHeaderLabels(missions);

	}
	if (m_ticketTable != NULL)
	{
		m_ticketTable->setColumnCount(2);
		m_ticketTable->resizeColumnsToContents();
		QStringList tickets;
		tickets<<"ִ����"<<"����ʱ��";
		m_ticketTable->setHorizontalHeaderLabels(tickets);
	}

	if (m_crateTime != NULL)
	{
		m_crateTime->setDateTime(QDateTime::currentDateTime());
	}

}