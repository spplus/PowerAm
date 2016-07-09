#include "ticketmgr.h"
#include "comutil.h"
TicketMgr::TicketMgr(QWidget* parent /* = NULL */)
{
	m_ticketTable = NULL;
	m_missionTable = NULL;
	m_crateTime = NULL;

	switch (ComUtil::instance()->getCurUserRole())
	{
	case 1://管理员
		break;
	case 2://调度
		initUi_Mission();
		break;
	case 3://自动化
		break;
	case 4://运维
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
	gbox->setTitle(tr("操作任务"));
	m_ticketTable = new QTableWidget;
	QHBoxLayout* hbox0 = new QHBoxLayout;
	hbox0->addWidget(m_ticketTable);
	gbox->setLayout(hbox0);

	QHBoxLayout* hbox1 = new QHBoxLayout;
	QLabel* doUser = new QLabel(tr("执行人"));
	m_userBox = new QComboBox;
	QLabel* doTime = new QLabel(tr("发令时间"));
	m_crateTime = new QDateTimeEdit;
	m_crateTime->setCalendarPopup(true);
	hbox1->addStretch();
	hbox1->addWidget(doUser);
	hbox1->addWidget(m_userBox);
	hbox1->addWidget(doTime);
	hbox1->addWidget(m_crateTime);
	hbox1->addStretch();

	QHBoxLayout* hbox2 = new QHBoxLayout;
	m_addBtn = new QPushButton(tr("新增"));
	m_saveBtn = new QPushButton(tr("保存"));
	m_delBtu = new QPushButton(tr("删除"));

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
	gbox->setTitle(tr("操作任务"));
	m_ticketTable = new QTableWidget;
	QHBoxLayout* hbox0 = new QHBoxLayout;
	hbox0->addWidget(m_ticketTable);
	gbox->setLayout(hbox0);

	QHBoxLayout* hbox1 = new QHBoxLayout;
	
	QGroupBox* gbox2 = new QGroupBox;
	gbox2->setTitle(tr("操作票"));
	m_missionTable = new QTableWidget;
	hbox1->addWidget(m_missionTable);
	gbox2->setLayout(hbox1);


	QHBoxLayout* hbox2 = new QHBoxLayout;
	m_createBtn = new QPushButton(tr("创建操作票"));
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
		missions<<"编号"<<"操作类型"<<"操作人"<<"监护人"<<"值班负责人"<<"操作开始时间"<<"操作结束时间"<<"备注";
		m_missionTable->setHorizontalHeaderLabels(missions);

	}
	if (m_ticketTable != NULL)
	{
		m_ticketTable->setColumnCount(2);
		m_ticketTable->resizeColumnsToContents();
		QStringList tickets;
		tickets<<"执行人"<<"发令时间";
		m_ticketTable->setHorizontalHeaderLabels(tickets);
	}

	if (m_crateTime != NULL)
	{
		m_crateTime->setDateTime(QDateTime::currentDateTime());
	}

}