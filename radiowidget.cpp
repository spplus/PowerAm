
#include "radiowidget.h"

RadioWidget::RadioWidget(QWidget* parent/* = NULL */)
{
	initUi();
	initSlots();
}

void RadioWidget::initUi()
{
	QHBoxLayout* hbox = new QHBoxLayout;
	
	char* pname = "ID";
	m_realtimeBtn = new QRadioButton(tr("ÊµÊ±Ì¬"));
	m_realtimeBtn->setChecked(true);

	m_analogBtn = new QRadioButton(tr("Ä£ÄâÌ¬"));
	
	m_ticketBtn = new QRadioButton(tr("ÄâÆ±Ì¬"));
	

	m_realtimeBtn->setIcon(QIcon(ICON_REALTIME));
	m_analogBtn->setIcon(QIcon(ICON_ANALOG));
	m_ticketBtn->setIcon(QIcon(ICON_TICKET));
	hbox->addWidget(m_realtimeBtn);
	hbox->addWidget(m_analogBtn);
	hbox->addWidget(m_ticketBtn);

	this->setLayout(hbox);
}

void RadioWidget::initSlots()
{
	connect(m_realtimeBtn,SIGNAL(pressed()),this,SIGNAL(realtime()));
	connect(m_analogBtn,SIGNAL(pressed()),this,SIGNAL(analog()));
	connect(m_ticketBtn,SIGNAL(pressed()),this,SIGNAL(ticket()));
}