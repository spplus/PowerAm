#include "ticketmgr.h"

TicketMgr::TicketMgr(QWidget* parent /* = NULL */)
{
	//initUi_Mission();
	initUi_Action();
	initSlots();
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
	m_saveBtn = new QPushButton(tr("保存"));
	//m_createBtn = new QPushButton(tr("创建操作票"));
	hbox2->addWidget(m_saveBtn);
	//hbox2->addWidget(m_createBtn);

	vbox->addWidget(gbox);
	vbox->addLayout(hbox1);
	vbox->addLayout(hbox2);

	connect(m_saveBtn,SIGNAL(pressed()),this,SLOT(onSaveTicket()));
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

void TicketMgr::onSaveTicket()
{

}